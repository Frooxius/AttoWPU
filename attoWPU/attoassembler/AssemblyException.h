#pragma once
#include "Exception.h"
#include "MessageStrings.h"

namespace attoWPU
{
	namespace assembler
	{
		class AssemblyException : Exception
		{
		public:
			AssemblyException(const char *msg)
				: Exception(msg, false)
			{ }

			AssemblyException(msgCode msg)
				: Exception(msgstrings[msg], false)
			{ }
		};
	}
}
