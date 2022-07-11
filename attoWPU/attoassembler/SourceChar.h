#pragma once
#include "SourceRead.h"
#include <string>

// This class allows reading a source code from a string in the memory

namespace attoWPU
{
	namespace assembler
	{
		class SourceChar : public SourceRead
		{
		private:
			char _ch;	// the character
			bool empty;	// determines if it has been read or not

		protected:
			virtual char __read();

		public:
			virtual bool Empty();
			SourceChar(char ch);
		};
	}
}