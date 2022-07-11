#include "05_OUT.h"

namespace attoWPU
{
	namespace simulator
	{
		Bits Unit05_OUT::ReadData()
		{
			if(DO)
				return Bits(DT, 0xFFFFFFFFU);
			else
				return Bits();
		}
	}
}