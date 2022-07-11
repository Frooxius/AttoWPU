#include "02_TEMP.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit02_TEMP::Unit02_TEMP() : Unit(0x02)
		{
			Reset(false);
		}

		void Unit02_TEMP::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
		}

		void Unit02_TEMP::Execute(byte code)
		{
			// remove invalid bits - 4 valid bits
			switch(code & 0x0F)
			{
				// stop
			case 0x00:
				// reset MO and DO - two lowest bits
				CR &= 0xFC;
				break;

				// write data with mask
			case 0x01:
				DT = DT&(~MK) | databus&MK;
				break;

				// output value with mask
			case 0x02:
				// set output and enable mask output
				CR = 0x06;
				break;

				// write data without mask
			case 0x03:
				// write data and ignore the mask
				DT = databus;
				break;

				// output value without mask
			case 0x04:
				// set output data and disable mask
				CR = 0x02;
				break;

				// write mask
			case 0x05:
				MK = databus;
				break;

				// output mask
			case 0x06:
				// set mask output, ignoring ME bit
				CR |= 0x01;
				// disable DO if set
				CR &= 0xFD;
				break;

				// enable mask
			case 0x07:
				CR |= 0x04; // set ME
				break;

				// disable mask
			case 0x08:
				CR &= 0xFB; // reset ME
				break;
				
				// clear data
			case 0x09:
				DT = 0;
				break;

				// fill data
			case 0x0A:
				DT = 0xFFFFFFFFU;
				break;
			}
		}

		Bits Unit02_TEMP::ReadData()
		{
			unsigned int tmp_mask = 0xFFFFFFFFU;
			if(MO())
				return Bits(MK, 0xFFFFFFFFU);
			if(ME())
				tmp_mask = MK;
			if(DO())
				return Bits(DT, tmp_mask);
			return Bits();
		}

		void Unit02_TEMP::Reset(bool hard)
		{
			databus = 0;
			DT = 0;
			MK = 0xFFFFFFFFU;
			CR = 0;
		}
	}
}