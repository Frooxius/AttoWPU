#pragma once
#include <string>
#include <stack>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include "Message.h"
#include "MessageStrings.h"
#include "AttocodeWrite.h"
#include "AssemblyException.h"
#include "InternalException.h"
#include "Source.h"
#include "SourceRead.h"
#include "SourceString.h"
#include "SourceChar.h"
#include "SourceFile.h"
#include "AttoASMHeader.h"
#include "csettings.h"

/*
	This class specifies the attoasm object, which controls whole assembly process.
*/

namespace attoWPU
{
	namespace assembler
	{
		class AttoASM;	// prototype
		
		const char SYMBOL_END = 0x1D;	// character representing the end of a symbol
		const char SYMBOL_OUT = 0x1E;	// character indicating to leave the symbol

		enum AttoASMFlags {
			 srcGenerated = 1
		};

		struct ProcessingPhase
		{
			void (AttoASM::*func)();	// processing function
			int phase;	// phase of the current funciton

			ProcessingPhase(void (AttoASM::*f)())
			{
				phase = 0;
				func = f;
			}
		};

		class AttoASM
		{
		private:
			// struct to bind together a number and pointer to a source, for local symbol name generator
			struct UniqName
			{
				unsigned int n;
				SourceRead *s;
				UniqName(unsigned int num, SourceRead *source) { n = num; s = source; }
			};

			Source *src;		// stack holding the source codes
			AttocodeWrite *trg;	// target for the assembled attocode

			std::stack<ProcessingPhase> processing;	// class containing processing functions
			std::map<std::string, std::string> symbols;	// contains all symbols
			std::list<std::string> reservedLabels;	// symbol names reserved for labels
			std::stack<std::string *> labels;	// contains pieces of code containing labels, for assembling them after everything else is assembled
			std::vector<long long> arg;	// holds return values from various functions

			char schar;				// currently loaded character from the source
			std::vector<std::string> sbuffer; // buffer for various purposes
			std::string 	statement,	// buffer for current statement
							processed,	// storage for the processed code
							lastsymbol;	// name of the last symbol loaded

			// Utility functions
			long long Number(std::string num);	// convert string to a number
			char LoadChar(bool preserveCase = false);	// load next character

			unsigned int namepool;	// pool for getting unique numbers for local symbols
			std::stack<UniqName> localnames;	// uniqe names of the local symbols - stay same for one level of a symbol
			void EnterSymbol();
			void LeaveSymbol();
			bool leaveSymbol;		// calling LeaveSymbol() is delayed by one step
			std::string GetUniqueName();
			bool IsLabel(std::string *name);

			// Working with symbols
			bool unknownLabel;	// determines if unknown label was encountered in the statement
			int statementAddress;	// starting address of the current statement
			std::string *GetSymbol(std::string *name);				// return symbol value
			void SetSymbol(std::string *name, std::string *value, bool redef = false, bool label = false);	// create new symbol

			void AssemblyCycle();	// a single assembly cycle
			void StatementEnd();	// needs to be called when statement is finished

			attoWPU::shared::Message *msgTarget; // object used to send messages to the output

			// statistics
			unsigned int statAttoinstructions, statData, statLine;

			// DEBUG
			void (AttoASM::*prevProcessing)();	// previous processing function

		public:
			AttoASM(SourceRead *initial, AttocodeWrite *target, attoWPU::shared::Message *msg);	// initialize assembly process

			void Assembly();			// assembly source completely in one call
			bool Assembly(int cycles);	// process a limited number of cycles of the assembly process in one call (several calls are required to complete the process)
			bool Done();	// returns whether the process is finished or not

			/*	The following is list of processing functions, that process the
				input source code and convert it to a machine code or a number,
				that's used by different function.
			*/

		private:
			void ProcessBase();			// the base of processing
			void ProcessExpression();	// process an expression
			void ProcessInsGroup();		// process an instruction group
			void ProcessInsRepeat();	// process attoinstructions
			void ProcessGroupRepeat();	// process instruction group repeats
			void ProcessNumber();		// decode a number
			void ProcessSymbol();		// decode a symbol
			void ProcessComment();		// process a comment
			void ProcessNumToIns();		// process number to attoinstruction
			void ProcessHexDataChunk();	// process a data chunk in hexadecimal
			void ProcessStrDataChunk();	// process a data chunk as string
			void ProcessOrg();			// process data organization
			void ProcessSymbolDef();	// process a symbol definition
			void ProcessInclude();		// process file include (both text and binary)
		};
	}
}