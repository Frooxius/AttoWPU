#include "0D_InputController.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit0D_InputCtrl::Unit0D_InputCtrl() : Unit(0x0D)
		{
			switches[0] = switches[1] = switches[2] = switches[3] = 0;
			Reset(false);
		}

		void Unit0D_InputCtrl::Reset(bool hard)
		{
			CR = TD = 0;
			for(int i = 0; i < 37; i++)
				keystatus[i] = false;
		}

		void Unit0D_InputCtrl::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
		}

		void Unit0D_InputCtrl::Execute(byte code)
		{
			switch(code & 0x0FU)
			{
				// stop data output
			case 0:
				CR &= 0xFE;	// clear DO
				break;

				// start data output
			case 1:
				CR |= 0x01;	// set DO
				break;

				// read switch row
			case 2:
			case 3:
			case 4:
			case 5:
				TD = switches[(code & 0x0FU)-2];
				CR = 0x01;
				break;

				// read numkey with no skip
			case 6:
				TD = ReadNum(0);
				CR = 0x03;
				break;
				// read numkey with skip
			case 7:
				TD = ReadNum(databus & 0xFFU);
				CR = 0x03;
				break;

				// read key with no skip
			case 8:
				TD = ReadKey(0);
				CR = 0x03;
				break;

				// read key with skip
			case 9:
				TD = ReadKey(databus & 0xFFU);
				CR = 0x03;
				break;
			}
		}

		Bits Unit0D_InputCtrl::ReadData()
		{
			if(!DO())
				return Bits();

			if(BM())
				return Bits(TD, 0xFFU);
			else
				return Bits(TD, 0xFFFFFFFFU);
		}

		void Unit0D_InputCtrl::SetNumKey(unsigned int id, bool status)
		{
			if(id > 10)
				return;

			keystatus[id] = status;
		}

		void Unit0D_InputCtrl::SetAlphaKey(unsigned int id, bool status)
		{
			if(id > 26)
				return;

			keystatus[id+10] = status;
		}

		void Unit0D_InputCtrl::SetSwitches(unsigned int row, unsigned int data)
		{
			if(row > 4)
				return;
			
			switches[row] = data;			
		}

		void Unit0D_InputCtrl::SetSwitch(unsigned int row, unsigned int num, bool status)
		{
			if(row > 4)
				return;
			unsigned int mask = 0x01U << num;	// prepare the universal mask
			// apply mask according to status
			if(status)
				switches[row] |= mask;	// set bit
			else
				switches[row] &= ~mask;	// clear bit			
		}

		unsigned int Unit0D_InputCtrl::ReadNum(unsigned int skip)
		{
			for(int i = 0; i < 11; ++i)
				if(keystatus[i])
				{
					if(skip--)
						continue;
					return i+1;
				}
			return 0;
		}

		unsigned int Unit0D_InputCtrl::ReadKey(unsigned int skip)
		{
			for(int i = 0; i < 26; i++)
				if(keystatus[i+11])
				{
					if(skip--)
						continue;
					return i+1;
				}
			return 0;
		}
	}
}