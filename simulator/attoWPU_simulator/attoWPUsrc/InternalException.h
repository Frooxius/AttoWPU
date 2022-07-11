#pragma once
#include "Exception.h"
#include "messagestrings.h"

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
