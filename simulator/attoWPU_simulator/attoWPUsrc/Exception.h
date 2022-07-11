#pragma once 
#include <string>

namespace attoWPU
{
	namespace assembler
	{
		class Exception
		{
		private:
			std::string __msg;	// string holding the message
			bool intern;	// determines if the exception is internal or not

		public:
			Exception(const char *msg, bool intern = false)
			{
				if(intern)
					__msg = "INTERNAL PROGRAM ERROR: ";
				__msg += msg;
			}

			std::string GetMessage() { return __msg; }

			bool Internal() { return intern; }
		};
	}
}