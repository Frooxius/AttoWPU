#include "SourceString.h"

namespace attoWPU
{
	namespace assembler
	{
		SourceString::SourceString(char *str, std::string name)
			: SourceRead(name)
		{
			cstring = true;
			cstr = str;
			_free = 0;
		}

		SourceString::SourceString(std::string *str, bool free, std::string name)
			: SourceRead(name)
		{
			cstring = false;
			stlstr = str->begin();
			stlend = str->end();
			if(free)
				_free = str;
			else
				_free = 0;
		}

		char SourceString::__read()
		{
			if(cstring)
				return *(cstr++);
			return *(stlstr++);
		}

		bool SourceString::Empty()
		{
			if(cstring)
				return *cstr == 0;
			return stlstr == stlend;
		}

		SourceString::~SourceString()
		{
			if(_free)
				delete _free;			
		}
	}
}