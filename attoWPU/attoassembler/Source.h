#pragma once
#include "SourceRead.h"
#include "SourceChar.h"
#include <string>
#include <stack>
#include <cstring>
#include <cstdlib>

/*
	Source objects holds one or more objects derived from SourceRead
	and allows reading single characters from them as well as whole
	strings. Objects are stored on a stack and when a character is
	read, it's always from the topmost object. When it's empty,
	then it's automatically popped out.
*/

namespace attoWPU
{
	namespace assembler
	{
		class Source
		{
		private:
			std::stack<SourceRead *> sources;	// stack for holding the sources			
			char __readTo(char *delim);		// internal function, which reads a single character if it's not
			bool endchar;	// if endchar was sent yet or not (it's sent after last sourcread is popped)

		public:
			Source();		// constructor creating an empty Source object
			Source(SourceRead *src);	// constructor creating Source with an initial SourceRead object

			void Push(SourceRead *src);	// pushes a new source object on the top
			SourceRead *Top() { if(!Empty()) return sources.top(); else { Push(new SourceChar('\n')); return Top(); } }
			char Read();				// reads a single character
			void ReadTo(char *buf, char *delim, int max = 255);	// read to a buffer until delimiter character is met
			void ReadTo(std::string *buf, char *delim);

			bool Empty();	// determines if it's empty (no SourceReads left on the stack)
			int Sources() { return sources.size(); }	// returns number of sources on the stack
		};
	}
}