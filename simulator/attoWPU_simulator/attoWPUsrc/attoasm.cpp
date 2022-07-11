#include "Attoasm.h"
#include <cctype>
#include <cstring>
#include <cmath>
#include <iostream>
#include "Values.h"

namespace attoWPU
{
	namespace assembler
	{
		using namespace shared;

		// Constructor - initializes the assembly process
		AttoASM::AttoASM(SourceRead *initial, AttocodeWrite *target, attoWPU::shared::Message *msg)
		{
			msgTarget = msg;
			msgTarget->Send(msgAttoassembling, msgInfo);

			src = new Source(initial);	// create the source stack with initial source
			src->Push(new SourceString(attoASMHeader));	// push the header souce, which will be processed first

			trg = target;

			// Push the base processing function
			processing.push(ProcessingPhase(&AttoASM::ProcessBase));

			// Push empty statement, to indicate, that labels weren't processed yet
			labels.push(new std::string(" "));

			// Create zero symbol, which is returned for unknown labels
			std::string *zero = new std::string("0");
			SetSymbol(zero, zero);
			delete zero;
			unknownLabel = false;
			statementAddress = 0;

			// initiate unique symbol name pool
			namepool = 1;
			localnames = std::stack<UniqName>();
			localnames.push(UniqName(0, 0));
			leaveSymbol = 0;

			// Load first character
			LoadChar();
		}

		bool AttoASM::Done()
		{
			return processing.empty();
		}

		void AttoASM::Assembly()
		{
			while(!Done())
				AssemblyCycle();
		}

		bool AttoASM::Assembly(int cycles)
		{
			while(!Done() && cycles--)
				AssemblyCycle();
			return Done();
		}

		void AttoASM::AssemblyCycle()
		{
			(this->*processing.top().func)();
		}

		// Utility functions

		long long AttoASM::Number(std::string num)
		{
			int digits, base, length = num.length();
			long long number = 0;	// must be stored as long, so it can be possibly used in expressions
			unsigned int overflow = 0;	// used to detect unsigned overflow
			bool minus = false;
			
			if(num.length() == 0)
				throw InternalException(msgErrNumberEmpty);

			// detect minus sign
			if(num[0] == '-')
				minus = true;

			//if the last character is a letter determine base
			switch(std::tolower(num[length-1]))
			{
			case 'h':
				base = 16;
				digits = length-1;
				break;
			case 'o':
				base = 8;
				digits = length-1;
				break;
			case 'b':
				base = 2;
				digits = length-1;
				break;
			case 'd':
				base = 10;
				digits = length-1;
				break;
			default:
				base = 10;
				digits = length;
			}

			if(minus)
				digits--;	//there's one less digit if the first character is a minus

			// detect zeroes in front that will be omitted
			int zeroes = minus;
			for(; zeroes < length-1 && num[zeroes] == '0'; zeroes++);
			zeroes -= minus;

			// create the number
			int multiplier = minus?-1:1;
			for(; digits > zeroes; digits--, multiplier *= base)
			{
				char dig = std::tolower(num[digits+minus-1]);
				int n;
				
				if(dig >= '0' && dig <= '9')
					n = dig-'0';
				else if(dig >= 'a' && dig <= 'f')
					n = 10+dig-'a';
				else
					throw AssemblyException(msgErrNumberUnexpectedSymbol);

				if(n > base-1)
					throw AssemblyException(msgErrNumberSmallBase);

				number += n*multiplier;

				// Detect overflow
				if((minus && (int)number > 0) || (!minus && (unsigned int)number < overflow) || multiplier == 0)
					throw AssemblyException(msgErrNumberTooLarge);
				overflow = number;
			}

			return number;
		}

		char AttoASM::LoadChar(bool preserveCase)
		{
			bool primary = (src->Sources() == 1),
				 source = src->Top() == localnames.top().s;

			if(preserveCase)
				schar = src->Read();
			else
				schar = std::tolower(src->Read());

			if(DEBUG)
				std::cout << schar;

			if(schar == SYMBOL_END)
			{
				if(processing.top().func != &AttoASM::ProcessSymbol)
					schar = ' ';
			}
			else if(schar == SYMBOL_OUT)
			{
				LeaveSymbol();
				schar = ' ';
			}


			return schar;
		}

		void AttoASM::StatementEnd()
		{
			// check if it's called correctly
			if(processing.size() != 1)
				throw InternalException(msgErrPrematureStatement);

			// Detect unknown label
			if(unknownLabel)
			{
				std::string *stat = new std::string("<");

				// first write organization label with the start of the statement with unknown label
				*stat += IntToStr(statementAddress);
				*stat += "> ";
				// then add the statement itself
				*stat += statement;
				// don't forget space on the end to terminate it properly
				*stat += ' ';
				// push it into unknown labels stack
				labels.push(stat);

#if DEBUG == 1
				std::cout << "STATEMENT SAVED: " << stat << std::endl;
#endif

				unknownLabel = false;
			}

			processed += statement;
			lastsymbol = "0";	// clear the last symbol used

			msgTarget->Send(msgDbgStatementEnd, msgInfo, true);

			statement.clear();	// clear statement (if there's any left)
			statement += schar;	// last character loaded has always to be preserved
			statementAddress = trg->GetAddress();
		}

		std::string *AttoASM::GetSymbol(std::string *name)
		{
			msgTarget->Send("GET: " + *name + "\n", msgInfo, true);

			std::map<std::string, std::string>::iterator i = symbols.find(*name);
			if(i == symbols.end())
			{
				if(labels.empty())
					throw AssemblyException(msgErrUndefSymbol);

				//	reserve symbol for a label (so it' can't be defined as normal symbol)
				if(std::find(reservedLabels.begin(), reservedLabels.end(), *name) == reservedLabels.end())
					reservedLabels.push_back(*name);

				// if it's not defined yet, consider it a label
				unknownLabel = true;
				return &symbols["0"];
			}
			else
				return &((*i).second);	// return the symbol value
		}

		void AttoASM::SetSymbol(std::string *name, std::string *value, bool redef, bool label)
		{
			if(DEBUG)
				std::cout << "SET: " << *name << std::endl;

			if(symbols.find(*name) != symbols.end())
			{
				// if redefinition is allowed, simply delete the current version
				if(redef)
					symbols.erase(*name);
				else
					throw Exception("Symbol/label redefinition.");
			}

			if(!label && IsLabel(name))
				throw Exception("Trying to use symbol before its definition or trying to define a label as arbitrary symbol.");

			// Create the symbol
			symbols.insert(std::pair<std::string, std::string>(*name, *value + " " + SYMBOL_END + SYMBOL_OUT));

			if(label)
				reservedLabels.push_back(*name);
		}

		bool AttoASM::IsLabel(std::string *name)
		{
			return (std::find(reservedLabels.begin(), reservedLabels.end(), *name) != reservedLabels.end());
		}

		void AttoASM::EnterSymbol()
		{
			if(DEBUG)
			std::cout << "Symbol entered: " << namepool  << " ( " << src->Sources() << " )" << std::endl;
			localnames.push(UniqName(namepool++, src->Top()));
		}

		void AttoASM::LeaveSymbol()
		{
			localnames.pop();
			if(DEBUG)
			std::cout << "Symbol left: " << localnames.top().n << " ( " << src->Sources() << " )" <<  std::endl;
		}

		std::string AttoASM::GetUniqueName()
		{
			std::string name = std::string("__");
			name += IntToStr(localnames.top().n);
			name += "__";
			return name;
		}

		/*	****************************************
					PROCESSING FUCTIONS
		*******************************************/

		void AttoASM::ProcessBase()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessBase__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			// If processing is finished
			if(src->Empty())
			{
				if(labels.empty())
				{
					// end the assembly process
					processing.pop();
					return;
				}
				else
				{
					// initiate unknown labels processing
					while(!labels.empty())
					{
						src->Push(new SourceString(labels.top(), true));
						labels.pop();
					}

					// load first character so it's immediatelly processed
					LoadChar();
				}
			}

			if(schar == ' ' || schar == '\n' || schar == '\r' || schar == '\f' || schar == '\t')
			{
				LoadChar();
				return;
			}

			if((schar >= '0' && schar <= '9') || schar == '-')
			{
				processing.push(ProcessingPhase(&AttoASM::ProcessInsGroup));
				return;
			}

			if(schar == '/')
			{
				processing.push(ProcessingPhase(&AttoASM::ProcessComment));
				return;
			}

			if(schar == '$')
			{
				processing.push(ProcessingPhase(&AttoASM::ProcessHexDataChunk));
				return;
			}

			if(schar == '"')
			{
				processing.push(ProcessingPhase(&AttoASM::ProcessStrDataChunk));
				return;
			}

			if(schar == '<')
			{
				processing.push(ProcessingPhase(&AttoASM::ProcessOrg));
				return;
			}

			if( (schar >= 'a' && schar <= 'z') || schar == '_')
			{
				processing.push(ProcessingPhase(&AttoASM::ProcessSymbol));
				return;
			}

			throw Exception("Unexpected symbol.");
		}

		// -------------------------------------
		void AttoASM::ProcessExpression()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessExpression__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			case 0:
				arg.push_back(0);	// start with a zero
				processing.top().phase = 2;	// move to the adding phase (loading numbers)
				// load the first number
				processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
				break;
			// process more numbers
			case 1:
				switch(schar)
				{
				// skip spaces
				case ' ':
				case '\n':
				case '\r':
				case '\f':
				case '\t':
					LoadChar();
					break;
				case '+':
					LoadChar();		// the + character needs to be omitted
					statement += '+';
					processing.top().phase++; // move to the next phase beforehand, where the value is added to the previous one
					processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
					break;
				case '-':
					processing.top().phase++; // move to the next phase beforehand, where the value is added to the previous one
					statement += '-';
					processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
					break;
				// anything else ends the expression
				default:
					statement += ' ';
					processing.pop(); // return back
				}
				break;
			// add the new number to the previous one
			case 2:
				arg[arg.size()-2] += arg[arg.size()-1];

				// add appropriate source data into the statement
				if(IsLabel(&lastsymbol))
					statement += lastsymbol;
				else
					statement += IntToStr(arg[arg.size()-1]);

				lastsymbol = "0";

				arg.pop_back();
				processing.top().phase--; // return to the previous phase
				break;
			}
		}

		// -------------------------------------
		void AttoASM::ProcessInsGroup()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessInsGroup__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// first phase decodes the start bit
			case 0:
				processing.top().phase++;
				sbuffer.push_back(std::string());	// prepare the sbuffer for storing the instruction group for repeats
				// this will decode the start bit and push it on the argument stack
				processing.push(ProcessingPhase(&AttoASM::ProcessExpression));
				break;
			// second phase stores the start bit in the sbuffer
			case 1:
				sbuffer[sbuffer.size()-1] += IntToStr(arg[arg.size()-1]);
				sbuffer[sbuffer.size()-1] += ' ';
				processing.top().phase++; // ensure that next time this phase won't be executed

				/*	the lack of break statement is delibrate, next phase should be executed
					immediatelly too, without having to wait for the next cycle */

			// third phase looks for a number of repeats
			case 2:
				switch(schar)
				{
				case ' ':
				case '\n':
				case '\r':
				case '\f':
				case '\t':
					LoadChar();
					break;
				// begining of the repeats specificaiton is found
				case '(':
					LoadChar();
					processing.top().phase++; // move to the next phase (which gets rid of spaces after repeats specification)
					processing.push(ProcessingPhase(&AttoASM::ProcessGroupRepeat));
					break;
				// if start of an instruction is found or technically anything else, illegal characters are handled in the next phase (no repeats are specified)
				default:
					processing.top().phase++; // move to the next
					arg.push_back(1);	// default number of repeats is one, when no repeats are specified
					break;
				}
				break;

			// get rid of additional spaces and detect symbol
			case 3:
				if(schar == ' ' || schar == '\n' || schar == '\r' || schar == '\f' || schar == '\t')
					LoadChar();
				else if((schar >= 'a' && schar <= 'z') || schar == '_')	// detect symbol
					processing.push(ProcessingPhase(&AttoASM::ProcessSymbol));
				else
					processing.top().phase++; // start the actuall instruction processing
				break;

			/*	in fifth phase, instructions themselves are processed
				arg now vector holds crutial values prepared in previous phases:
					arg[size-2] - current bus bit
					arg[size-1] - number of group repeats			*/
			case 4:
				switch(schar)
				{
				case '0':
					trg->Write(attoFall, arg[arg.size()-2]++);
					sbuffer[sbuffer.size()-1] += schar;

					// if this is a non-generated source, save the instructions into the statement
					if( !(src->Top()->flags & srcGenerated))
						statement += '0';

					LoadChar();
					break;
				case '1':
					trg->Write(attoRise, arg[arg.size()-2]++);
					sbuffer[sbuffer.size()-1] += schar;

					if( !(src->Top()->flags & srcGenerated))
						statement += '1';

					LoadChar();
					break;
				case '!':
					trg->Write(attoNot, arg[arg.size()-2]++);
					sbuffer[sbuffer.size()-1] += schar;

					if( !(src->Top()->flags & srcGenerated))
						statement += '!';

					LoadChar();
					break;
				case '|':
					trg->Write(attoHalt, arg[arg.size()-2]++);
					sbuffer[sbuffer.size()-1] += schar;

					if( !(src->Top()->flags & srcGenerated))
						statement += '|';

					LoadChar();
					break;
				case '-':
					arg[arg.size()-2]++;
					sbuffer[sbuffer.size()-1] += schar;

					if( !(src->Top()->flags & srcGenerated))
						statement += '-';

					LoadChar();
					break;

				// start of the instruction repeats specification
				case '(':
					processing.push(ProcessingPhase(&AttoASM::ProcessInsRepeat));
					LoadChar();
					break;

				// number to instruction conversion
				case '[':
					processing.push(ProcessingPhase(&AttoASM::ProcessNumToIns));
					break;

				// end of the instructions (and whole statement)
				case ' ':
				case '\n':
				case '\r':
				case '\f':
				case '\t':
					// first, push appropriate number of group repeats, they are processed as separate statements
					sbuffer[sbuffer.size()-1] += ' ';	// add space to terminate the statement in the sbuffer
					while(--arg[arg.size()-1])
						src->Push(new SourceString(new std::string(sbuffer[sbuffer.size()-1]), true));

					// clean up
					arg.pop_back();		// repeats
					arg.pop_back();		// bus bit
					processing.pop();	// return to process base
					sbuffer.pop_back();
					
					StatementEnd();

					break;

				// illegal instruction symbol
				default:
					throw Exception("Illegal instruction symbol.");
				} // END switch(schar)
				break;
			} // END switch(processing.top().phase)
		} // END void AttoASM::ProcessInsGroup()

		// -------------------------------------
		void AttoASM::ProcessInsRepeat()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessInsRepeat__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			std::string *ins;  // for storing repeated instruction

			switch(processing.top().phase)
			{
			case 0:
				// first load the number
				processing.top().phase++;
				processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
				break;
			case 1:
				// check if there's ) character, which terminates repeat statement
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;

				case ')':
					// push appropriate number of attoinstructions to the source stack
					ins = new std::string();
					if(arg[arg.size()-1] <= 0)
						throw Exception("Invalid number of instruction repeats.");
						
					while(--arg[arg.size()-1])
						*ins += sbuffer[sbuffer.size()-1][sbuffer[sbuffer.size()-1].length()-1];
					// push them to the source stack
					src->Push(new SourceString(ins, true));

					// clean up
					arg.pop_back();
					processing.pop(); // return to the previous
					LoadChar(); // remove ')' from the source

					break;

				// syntax error
				default:
					throw Exception("Unexpected symbol. Right parenthesis ')' expected.");
				} // END switch(schar)
				break;
			} // END switch(processing.top().phase)
		}

		// -------------------------------------
		void AttoASM::ProcessNumber()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessNumber__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}
			
			switch(processing.top().phase)
			{
			case 0:
				sbuffer.push_back(std::string()); // prepare the buffer for numbers
				processing.top().phase++;
				// lack of break is deliberate, so first number character is processed too in this step
			// in second phase, skip all spaces before the number
			case 1:
				// detect minus sign or a symbol
				if((schar >= 'a' && schar <= 'z') || schar == '_')
				{
					processing.push(ProcessingPhase(&AttoASM::ProcessSymbol));
					break;
				}

				switch(schar)
				{
				case '-':
					// it can also detect several consequent minuses
					if(sbuffer[sbuffer.size()-1].empty())
						sbuffer[sbuffer.size()-1] += '-';
					else
						sbuffer[sbuffer.size()-1].clear();
					// lack of break is deliberate so next character is loaded
				case ' ':
				case '\n':
				case '\r':
				case '\f':
				case '\t':
					LoadChar();
					break;
				default:
					processing.top().phase++; // move to the next phase
				}
				break;
			case 2:
				// load numbers into the sbuffer
				if(std::isalpha(schar) || std::isdigit(schar))
				{
					sbuffer[sbuffer.size()-1] += schar;
					LoadChar();
				}
				else
				{
					// process the number
					arg.push_back(Number(sbuffer[sbuffer.size()-1]));
					// clean up
					sbuffer.pop_back();
					processing.pop();
				}
				break;
			}
		}

		// -------------------------------------
		void AttoASM::ProcessGroupRepeat()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessGroupRepeat__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			case 0:
				// first load the number
				processing.top().phase++;
				processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
				break;
			case 1:
				// check if there's ) character, which terminates repeat statement
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;

				case ')':
					statement += "(";
					statement += IntToStr(arg[arg.size()-1]);
					statement += ") ";
					processing.pop(); // return to the previous
					LoadChar();
					break;

				// syntax error
				default:
					throw Exception("Unexpected symbol. Right parenthesis ')' expected.");
				} // END switch(schar)
				break;
			} // END switch(processing.top().phase)
		}

		// -------------------------------------
		void AttoASM::ProcessSymbol()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessSymbol__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// initialize loading the symbol
			case 0:
				sbuffer.push_back(std::string());	// create new buffer for the symbol name
				processing.top().phase++;		// start loading the symbol name
				// break is omitted deliberately
			// load symbol name
			case 1:
				if(std::isalpha(schar) || std::isdigit(schar) || schar == '_')
				{
					sbuffer[sbuffer.size()-1] += schar;
					LoadChar();
				}
				else if(schar == '%')
				{
					// local symbol - add name to the value
					sbuffer[sbuffer.size()-1] += GetUniqueName();

					LoadChar();
				}
				else
					processing.top().phase++;
				break;
			// determine what to do with the name
			case 2:
				switch(schar)
				{
				// skip all spaces
				case ' ':
				case '\n':
				case '\r':
				case '\f':
				case '\t':
					LoadChar();
					break;
				// symbol definition
				case '{':
					processing.pop();	// this function's work is done and won't be called after symbol definition function is processed
					processing.push(ProcessingPhase(&AttoASM::ProcessSymbolDef));
					break;
				// label definition
				case ':':
					LoadChar(); // remove ':' from the source
					// create symbol with corresponding address
					SetSymbol(&sbuffer[sbuffer.size()-1], &IntToStr(trg->GetAddress()), false, true);
					// cleanup
					sbuffer.pop_back();
					processing.pop();
					StatementEnd();

					break;

				// anything else is symbol usage - symbol value is pushed into the source
				default:
					lastsymbol = sbuffer[sbuffer.size()-1];
							
					// detect special symbols (includes)
					if(sbuffer[sbuffer.size()-1] == "include" || sbuffer[sbuffer.size()-1] == "binclude")
					{
						processing.pop();	// this function won't need to be executed after include ones are finished
						processing.push(ProcessingPhase(&AttoASM::ProcessInclude));
						if(sbuffer[sbuffer.size()-1][0] == 'b')
							arg.push_back(1);	// determines that binary include will be created							
						else
							arg.push_back(0);	// text include will be created

						//cleanup
						sbuffer.pop_back(); // the symbol name isn't needed anymore
						break;
					}

					// the last character loaded needs to be preserved and read after the symbol
					src->Push(new SourceChar(schar));
					// push the value of the symbol onto the source
					src->Push(new SourceString(GetSymbol(&sbuffer[sbuffer.size()-1])));

					EnterSymbol();	// get new unique name

					LoadChar(); // load next character (to get rid of previous schar, which is preserved using SourceChar)
					// cleanup
					sbuffer.pop_back();
					processing.pop();
				}
			}
		}

		// -------------------------------------
		void AttoASM::ProcessComment()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessComment__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// determine type of the comment
			case 0:
				LoadChar();
				switch(schar)
				{
				// single line
				case '/':
					processing.top().phase = 1;	// second phase handles single line comment
					break;
				// multi line
				case '*':
					processing.top().phase = 2;	// third phase handles multiline comment
					break;
				// syntax error
				default:
					throw Exception("Unexpected symbol, '/' or '*' expected.");
				}
				break;
			// handle single line comment
			case 1:
				LoadChar();
				if(schar == '\n' || schar == '\r' || schar == '\f' || schar == '\t')
				{
					processing.pop();
					StatementEnd();
				}
				break;
			// handle multiline comment
			case 2:
				if(LoadChar() == '*')
					processing.top().phase = 3; // fourth phase detects end of the multiline comment
				break;

			case 3:
				if(LoadChar() == '/')
				{
					processing.pop();
					LoadChar();	// remove the '/' from the source
					StatementEnd();
				}
				else
					processing.top().phase = 2; // if it's false alarm, move back
				break;
			}
		}

		// -------------------------------------
		void AttoASM::ProcessNumToIns()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessNumToIns__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// load the number to convert
			case 0:
				LoadChar();	// load first number
				processing.top().phase++;	// ensure that next phase will be executed after expression is evaluated
				processing.push(ProcessingPhase(&AttoASM::ProcessExpression));

				statement += "[";
				break;
			// check if number of bits to convert is specified
			case 1:
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;
				// number of bits to covert are specified
				case ',':
					LoadChar(); // load first digit
					processing.top().phase++; // next phase must be executed after number is loaded
					processing.push(ProcessingPhase(&AttoASM::ProcessNumber));

					break;
				// only the value to convert is specified
				case ']':
					processing.top().phase = 3;	// move to the last phase

					// default values
					arg.push_back(32);	// 32 bits to convert
					arg.push_back(0);	// 0 bits to skip
					break;
				default:
					throw Exception("Unexpected symbol, ']' or ',' expected.");
				}
				break;
			// check if number of bits to skip is specified
			case 2:
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;
				// number of bits to skip are specified
				case ',':
					LoadChar(); // load first digit
					processing.top().phase++; // next phase must be executed after number is loaded
					processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
					break;
				// only the value and number of how many to convert is specified
				case ']':
					processing.top().phase = 3;	// move to the last phase
					// default values
					arg.push_back(0);	// 0 bits to skip

					break;
				default:
					throw Exception("Unexpected symbol, ']' or ',' expected.");
				}
				break;

			/* process loaded values and create attoinstructions
				arg-1: bits to skip
				arg-2: bits to convert
				arg-3: value to conhvert		*/
			case 3:
				statement += ", ";
				statement += IntToStr(arg[arg.size()-2]);
				statement += ", ";
				statement += IntToStr(arg[arg.size()-1]);
				statement += "] ";

				// because conversion is from left to right, values needs to be adjusted
				unsigned data = arg[arg.size()-3],
					bits = arg[arg.size()-2],
					skip = 32-arg[arg.size()-1]-arg[arg.size()-2];

				if(skip+bits > 32)
					throw Exception("Illegal values: can process maximum of 32 bits (skipped+processed <= 32).");

				sbuffer.push_back(std::string());	// for instructions
				// skip bits
				data <<= skip;
				// create instructions
				while(bits-- > 0)
				{
					if(data & 0x80000000)
						sbuffer[sbuffer.size()-1] += '1';
					else
						sbuffer[sbuffer.size()-1] += '0';
					data <<= 1;
				}
				sbuffer[sbuffer.size()-1] += ' ';	// end the statment
				// push the instructions onto the sources stack
				src->Push(new SourceString(new std::string(sbuffer[sbuffer.size()-1]), true));
				src->Top()->flags |= srcGenerated;	// this is an generated source and won't be saved into the statement
				LoadChar();	// load the first one

				// cleanup
				sbuffer.pop_back();
				arg.pop_back();	// skip
				arg.pop_back();	// bits
				arg.pop_back();	// data
				processing.pop();
				break;
			}
		}

		// -------------------------------------
		void AttoASM::ProcessHexDataChunk()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessHexDataChunk__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// initialize
			case 0:
				sbuffer.push_back(std::string());	// create buffer for storing hexadecimals
				sbuffer[sbuffer.size()-1] = '0';	// always put zero in front, because the number will be hexadecimal
				processing.top().phase++; // move to the next phase where characters are loaded
				// break is omitted deliberately
			case 1:
				LoadChar();
				if(std::isdigit(schar) || std::isalpha(schar))
				{
					sbuffer[sbuffer.size()-1] += schar;
					// if buffer is filled enough, write data
					if(sbuffer[sbuffer.size()-1].length() == 3)
					{
						// add h at the end, because the number is hexadecimal
						sbuffer[sbuffer.size()-1] += 'h';
						// write data
						try {
							trg->Write((byte)Number(sbuffer[sbuffer.size()-1]));
						} catch(Exception e)
						{ throw Exception("Illegal symbol in hexadecimal data chunk."); }
						// clear buffer for next round
						sbuffer[sbuffer.size()-1] = '0';
					}
				}
				else
				{
					// if one nibble is missing, push it to the source, so it's processed in next round
					if(sbuffer[sbuffer.size()-1].length() == 2)
					{
						src->Push(new SourceChar(schar)); // preserve already read character
						src->Push(new SourceChar('0'));
					}
					else
					{
						// it's finished, clean up
						sbuffer.pop_back();
						processing.pop();
						StatementEnd();
					}
				}
			}
		}

		// -------------------------------------
		void AttoASM::ProcessStrDataChunk()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessStrDataChunk__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// normal processing
			case 0:
				LoadChar(true);
				// end
				if(schar == '"')
				{
					processing.pop();
					LoadChar();
					StatementEnd();
				} else if(schar == '\\') // escaping
					processing.top().phase = 1; // move to the second phase, which determines escape character
				else
					trg->Write(schar);	// write character as is
				break;

			// escape characters
			case 1:
				switch(LoadChar(true))
				{
				case 'n':
					trg->Write('\n');
					break;
				case '"':
					trg->Write('"');
					break;
				case 't':
					trg->Write('\t');
					break;
				case '0':
					trg->Write(0);
					break;
				case '\\':
					trg->Write('\\');
					break;
				case 'f':
					trg->Write('\f');
					break;
				case 'r':
					trg->Write('\r');
					break;
				case 'b':
					trg->Write('\b');
					break;
				default:
					throw Exception("Unknown escape sequence.");
				}
				// move to the previous phase
				processing.top().phase = 0;
				break;
			}
		}

		// -------------------------------------
		void AttoASM::ProcessOrg()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessOrg__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			case 0:
				LoadChar();	// get rid of the '<'
				processing.top().phase++; // move to the next phase (will be executed after number is read)
				// load the number first
				processing.push(ProcessingPhase(&AttoASM::ProcessNumber));
				break;
			// detect end of the organization statement and change it
			case 1:
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;
				// End of the statement
				case '>':
					// if the address is larger than current one, appropriate number of zeroes must be written
					if(arg[arg.size()-1] > trg->GetTopAddress())
					{
						for(int i = 0; i < 100000 && (arg[arg.size()-1] > trg->GetTopAddress()); i++)
							trg->Write((char)0);
						break;
					}

					// set the address - number loaded in the first phase
					trg->SetAddress(arg[arg.size()-1]);

					// cleanup
					arg.pop_back();
					processing.pop();
					LoadChar(); // get rid of the '>'
					StatementEnd();
					break;

				// syntax error
				default:
					throw Exception("Unexpected symbol, '>' expected.");
				}
			}
		}

		// -------------------------------------
		void AttoASM::ProcessSymbolDef()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessSymbolDef__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// initialize
			case 0:
				LoadChar(true);	// load the first character of the symbol definition
				// Check if the first symbol is exclamation mark, which allows symbol redefinition
				if(schar == '!')
				{
					sbuffer.push_back(std::string("R"));	// character indicates, that symbol redefinition is allowed
					LoadChar(true); // load another character
				}
				else
					sbuffer.push_back(std::string());	// no character means, that no symbol redefinition is allowed


				sbuffer.push_back(std::string());	// sbuffer for the value
				processing.top().phase++;	// next phase which loads the symbol value
				
				arg.push_back(1); // how many } need to be read, before the symbol definition is closed

				// break is omitted deliberately - process first symbol after initialization
			// load the symbols as they are
			case 1:
				if(schar == '{')
					arg[arg.size()-1]++;	// increase number of the { required

				// TO DO LATER: what if it's inside a string?

				if(schar != '}')
				{
					// load the character
					sbuffer[sbuffer.size()-1] += schar;
					LoadChar(true);
				}
				else
				{
					// decrease the number of required {'s
					arg[arg.size()-1]--;

					if(arg[arg.size()-1] == 0)
					{
						// whole value is loaded, create symbol
						SetSymbol(&sbuffer[sbuffer.size()-3], &sbuffer[sbuffer.size()-1], sbuffer[sbuffer.size()-2].length() != 0);
						// cleanup
						LoadChar(); // remove the '}'
						sbuffer.pop_back();	// value
						sbuffer.pop_back();	// redefinition flag
						sbuffer.pop_back();	// name
						arg.pop_back(); // number of } left
						processing.pop();	// return to process base
						StatementEnd();
					}
					else
					{
						// load the character
						sbuffer[sbuffer.size()-1] += schar;
						LoadChar(true);
					}
				}
				break;
			}
		}

		// -------------------------------------
		void AttoASM::ProcessInclude()
		{
			if(DEBUG)
			{
				if(prevProcessing != processing.top().func)
				{
					std::cout << std::endl << "__ProcessInclude__" << std::endl;
					prevProcessing = processing.top().func;
				}
			}

			switch(processing.top().phase)
			{
			// initialize
			case 0:
				sbuffer.push_back(std::string()); // prepare sbuffer for loading filename
				processing.top().phase++;
				// break omitted deliberately
			case 1:
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;
				case '(':
					LoadChar();
					processing.top().phase++;
					break;
				default:
					throw Exception("Unexpected symbol in include statement, '(' expected.");
				}
				break;
			case 2:
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;
				case '"':
					LoadChar(true);
					processing.top().phase++;
					break;
				default:
					throw Exception("Unexpected symbol in include statement, '\"' expected.");
				}
				break;
			// load the filename
			case 3:
				if(schar != '"')
					sbuffer[sbuffer.size()-1] += schar;
				else
					processing.top().phase++;
				LoadChar(true);
				break;
			// check for proper ending
			case 4:
				switch(schar)
				{
				case ' ':
					LoadChar();
					break;
				case ')':
					if(arg[arg.size()-1])
					{
						// binary include
						// include file as usuall, but processing is done in sixth phase
						src->Push(new SourceFile(sbuffer[sbuffer.size()-1].c_str()));

						// store how many sources are there, so end of the binary source can be detected
						arg[arg.size()-1] = src->Sources();

						sbuffer.pop_back();	// filename isn't needed anymore
						processing.top().phase++;
					}
					else
					{
						// text include
						src->Push(new SourceChar(' '));	// always add space for safety
						src->Push(new SourceFile(sbuffer[sbuffer.size()-1].c_str()));
						EnterSymbol();	// get new unique name
						LoadChar();

						// cleanup
						processing.pop();
						sbuffer.pop_back();
						arg.pop_back();
						StatementEnd();
					}
					break;
				default:
					throw Exception("Unexpected symbol in include statement, ')' expected.");
				} // END switch(schar)
				break;
			// Sixth phase - process binary include
			case 5:
				// write raw data
				LoadChar(true);
				trg->Write(schar);

				// detect end of the binary file
				if(arg[arg.size()-1] != src->Sources())
				{
					// cleanup
					arg.pop_back();
					processing.pop();
					LoadChar(); // load next character from normal source
					StatementEnd();
				}

				break;
			}
		}
	}
}