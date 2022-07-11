#pragma once
#include "SourceRead.h"
#include <fstream>

//	This class allows reading a source code from a text file on the disk

namespace attoWPU
{
	namespace assembler
	{
		class SourceFile : public SourceRead
		{
		private:
			std::ifstream *file;
			char nextch;

		protected:
			virtual char __read();		// reading a single character

		public:
			virtual bool Empty();
			SourceFile(const char *filename);	// constructor, which is also used to specify a file
			~SourceFile();	// destructor used to close the file stream
		};
	}
}