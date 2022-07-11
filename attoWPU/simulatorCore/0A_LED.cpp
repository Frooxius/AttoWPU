#include "0A_LED.h"

namespace attoWPU
{
	namespace simulator
	{
		unsigned int Unit0A_LED::GetRow(int id)
		{
			// to do - check range
			return ROW[id];
		}
	}
}