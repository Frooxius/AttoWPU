#pragma once
#include "Exception.h"
#include "MessageStrings.h"

namespace attoWPU
{
	namespace assembler
	{
		class InternalException : Exception
		{
		public:
			InternalException(const char *msg)
				: Exception(msg, true)
			{ }

			InternalException(msgCode msg)
				: Exception(msgstrings[msg], true)
			{ }
		};
	}
}
