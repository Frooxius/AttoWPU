#pragma once
#include "Bits.h"

// This abstract class is used to write binary data to some location

namespace attoWPU
{
	namespace shared
	{
		class DataWrite
		{
		public:
			virtual void Write(byte data) = 0;
			virtual bool Full() = 0;
		};
	}
}