#include "SourceRead.h"
#include "Exception.h"
#include <cstring>
using namespace std;


namespace attoWPU
{
	namespace assembler
	{
		char SourceRead::Read()
		{
			char tmp;

			if(Empty())
				throw Exception("Trying to read from empty SourceRead.", true);

			tmp = __read();
			pos++;

			// detect new line
			if(tmp == '\n' || tmp == '\f' || tmp == '\r')
			{
				if(!newline)
				{
					pos = 0;
					line++;
					newline = true;
				}
			}
			else if(newline)
				newline = false;

			return tmp;
		}
	}
}