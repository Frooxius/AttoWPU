#include "00_aPC.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit00_aPC::Unit00_aPC() : Unit(0x00)
		{
			Reset(false);
		}

		void Unit00_aPC::Execute(byte code)
		{
			// remove invalid bits - 2 valid bits
			switch(code & 0x03)
			{
				// Stop
			case 0x00:
				CR &= 0xFE;	// null the AO bit
				break;

				// Write
			case 0x01:
				CR |= 0x02; // set the SP bit
				IA = databus & 0x00FFFFFF;	// write new address
				break;

				// Output
			case 0x02:
				// set the AO bit
				CR |= 0x01;
				break;

				// Reset
			case 0x03:
				// reset the IA value
				IA = 0;
				CR |= 0x02; // set the SP bit
				break;
			}
		}

		void Unit00_aPC::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
		}

		void Unit00_aPC::Pulse()
		{
			if(SP())
			{
				CR &= 0xFD;	// null the SP bit
				return;		// do not increment IA
			}
			// increment and test if it doesn't exceed 24 bit range
			if(++IA > 0x00FFFFFF)
				IA = 0;
		}

		Bits Unit00_aPC::ReadData()
		{
			if(AO())
				return Bits(IA, 0x00FFFFFF);	// return IA value
			else
				return Bits();	// return nothing
		}

		void Unit00_aPC::Reset(bool hard)
		{
			databus = 0;

			// set default values
			IA = 0;
			CR = 0x02;	// set the SP bit - so the address 000000 isn't skipped
		}

	}
}