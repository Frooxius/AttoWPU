#pragma once
#include <string>
/*
	SourceRead is an abstract class, that is used to read a single character or a whole
	keyword from an arbitrary source. The source itself is implemented by derived classes,
	and it's usually a text file or a string.
*/

namespace attoWPU
{
	namespace assembler
	{
		class SourceRead
		{
		private:
			std::string name;	// name of the source
			int line, pos;	// info about the possition in the file
			bool newline;

		protected:
			virtual char __read() = 0;		// internal function for reading a single character
			void SetName(std::string newname) { name = newname; }

		public:
			SourceRead(std::string name = "") { flags = pos = 0; line = 1; newline = false; }	// constructor

			std::string GetName() { return name; }
			int GetPos() { return pos; }
			int GetLine() { return line; }

			unsigned int flags;
			char Read();										// read a single character
			virtual bool Empty() = 0;	// returns true if the source is empty (everything is read)
			virtual ~SourceRead() { };
		};
	}
}