#include "Source.h"
#include "Exception.h"
#include "SourceString.h"
#include <iostream>

namespace attoWPU
{
	namespace assembler
	{
		Source::Source()
		{
			endchar = false;
		}

		Source::Source(SourceRead *src)
		{
			sources.push(src);
			endchar = false;
		}

		void Source::Push(SourceRead *src)
		{
			sources.push(src);
			endchar = false;	// reset end char when new source is pushed
		}

		char Source::Read()
		{
			if(Empty() && endchar)
				throw Exception("Trying to read from an empty Source.", true);
			else if(Empty())	// send endchar
			{
				endchar = true;
				return '\n';
			}

			char ch = sources.top()->Read();	// read character
			if(sources.top()->Empty())			// if this caused the source read to be empty, dispose of it
			{
				delete sources.top();
				sources.pop();
			}
			return ch;
		}

		char Source::__readTo(char *delim)
		{
			// if zero is returned, it automatically terminates the string
			if(Empty())
				return 0;
			char ch = Read();

			if(!strchr(delim, ch))	// if the character is not one of the delimiting ones, return it, otherwise zero
				return ch;
			return 0;
		}
		
		void Source::ReadTo(char *buf, char *delim, int max)
		{
			while(--max)
			{
				*buf = __readTo(delim);
				if(!(*(buf++)))		// if it's a terminating zero, return
					return;
			}
			*buf = 0;	// if no delimiter is found, add zero at the end automatically
		}

		void Source::ReadTo(std::string *buf, char *delim)
		{
			char ch;
			while(ch = __readTo(delim))
				*buf += ch;
		}

		bool Source::Empty()
		{
			return sources.empty();
		}


	}
}