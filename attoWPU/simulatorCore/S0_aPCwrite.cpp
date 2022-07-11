#include "S0_aPCwrite.h"

namespace attoWPU
{
	namespace simulator
	{
		void UnitS0_aPCWrite::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{

			// detect change of the jump bit
			if((ajmp&0x01) && !jumpbit)
			{
				// local jump
				ajmp >>= 1;	// get the offset
				_aPC->SetIA((_aPC->GetIA()&0xFF8000U) | ajmp);
				jumpbit = true;
			} else if(jumpbit && !(ajmp&0x01))
				jumpbit = false;
		}
	}
}