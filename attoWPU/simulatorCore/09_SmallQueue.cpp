#include "09_SmallQueue.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit09_SmallQueue::Unit09_SmallQueue() : Unit(0x09)
		{
			// initialize memory
			mem = new byte[256];
			Reset(true);
		}

		Unit09_SmallQueue::~Unit09_SmallQueue()
		{
			delete[] mem;
		}

		void Unit09_SmallQueue::Execute(byte code)
		{
			switch(code&0x1F)
			{
				// stop data output/input
			case 0x00:
				CR &= 0x88;	// null everything but QR and ME
				break;

				// write new address
			case 0x01:
				AD = (byte)databus;
				break;

				// output current address
			case 0x02:
				CR &= 0x88;
				CR |= 0x01;	// set AO
				break;

				// output addressed data with mask
			case 0x03:
				CR &= 0x80; // null evertyhing but QR
				CR |= 0x0A;	// set DO and ME
				break;

				// write data from the databus with mask
			case 0x04:
				*(mem+AD) = Bits(databus, MK).Replace(*(mem+AD));
				break;

				// move to the next element
			case 0x05:
				AD++;
				break;

				// move to the previous element
			case 0x06:
				AD--;
				break;

				// output addressed data (without mask)
			case 0x07:
				CR &= 0x80;	// null everything but QR
				CR |= 0x02; // set DO
				break;

				// write data from the databus (without mask)
			case 0x08:
				*(mem+AD) = databus;
				break;

				// enable mask
			case 0x09:
				CR |= 0x08;
				break;

				// disable mask
			case 0x0A:
				CR &= 0xF7;
				break;

				// enable data output mode
			case 0x0B:
				CR &= 0x88;
				CR |= 0x02;
				break;

				// enable data write mode
			case 0x0C:
				CR &= 0x88;
				CR |= 0x04;
				break;

				// set countdown
			case 0x0D:
				CD = 0x0000FFFF & databus;
				break;

				// set fill
			case 0x0E:
				FL = 0x0000FFFF & databus;
				break;

				// start
			case 0x0F:
				CR |= 0x80;
				break;

				// stop
			case 0x10:
				CR &= 0x7F;
				break;
			}
		}

		void Unit09_SmallQueue::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
			if(QR())
			{
				// when running
				if(!(CD--))
				{
					// when CD reaches zero

					// write new data
					if(DW())
						*(mem+AD) = Bits(databus, MK).Replace(*(mem+AD));
					
					if(AD != 0xFF)
						AD++;
					else
						CR &= 0x7F;	// stop the register when last address is reached

					// refill CR
					CR = FL;
				}
			}
		}

		Bits Unit09_SmallQueue::ReadData()
		{
			if(AO())
				return Bits(AD, 0x000000FFU);
			if(DO())
			{
				unsigned int tempmask = 0xFFFFFFFF;
				if(ME())
					tempmask = MK;
				return Bits(*(mem+AD), tempmask);
			}

			return Bits();
		}

		void Unit09_SmallQueue::Reset(bool hard)
		{
			CD = FL = AD = CR = 0;
			MK = 0xFFFFFFFFU;

			if(hard)
				for(int i = 0; i < 256; i++)
					*(mem+i) = 0x00;
		}
	}
}