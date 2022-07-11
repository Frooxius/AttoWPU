#pragma once
#include "SourceRead.h"
#include <string>

// This class allows reading a source code from a string in the memory

namespace attoWPU
{
	namespace assembler
	{
		class SourceString : public SourceRead
		{
		private:
			std::string::iterator stlstr, stlend; // iterator to a STL string
			std::string *_free;	// if set, it will be freed
			char *cstr;			// pointer to a C string
			bool cstring;		// determines whether to use C string or STL pointer

		protected:
			virtual char __read();

		public:
			virtual bool Empty();
			SourceString(char *str, std::string name = "");
			SourceString(std::string *str, bool free = false, std::string name = "");
			~SourceString();
		};
	}
}