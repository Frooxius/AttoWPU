#include "SourceChar.h"

namespace attoWPU
{
	namespace assembler
	{
		SourceChar::SourceChar(char ch)
		{
			_ch = ch;
			empty = false;
		}

		char SourceChar::__read()
		{
			empty = true;
			return _ch;
		}

		bool SourceChar::Empty()
		{
			return empty;
		}
	}
}