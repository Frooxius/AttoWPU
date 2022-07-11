#pragma once
#include "Bits.h"

// This abstract class is used to read binary data from some location

namespace attoWPU
{
	using namespace shared;
	namespace simulator
	{
		class DataRead
		{
		public:
			virtual byte Read() = 0;
			virtual bool Empty() = 0;
		};
	}
}