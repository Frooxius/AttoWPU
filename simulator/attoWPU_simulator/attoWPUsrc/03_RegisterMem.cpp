#include "03_RegisterMem.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit03_RegisterMem::Unit03_RegisterMem() : Unit(0x03)
		{
			mem = new unsigned int[256];	// allocate memory for data
			Reset(true);
		}

		void Unit03_RegisterMem::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
		}

		Unit03_RegisterMem::~Unit03_RegisterMem()
		{
			delete[] mem;
		}

		Bits Unit03_RegisterMem::ReadData()
		{
			if(DO())
			{
				// set mask
				unsigned int tmpmask = 0xFFFFFFFFU;
				if(ME())
					tmpmask = MK;
				return Bits(*(mem+AD), tmpmask);
			}

			if(AO())
				return Bits(AD, 0xFF);

			if(MO())
				return Bits(MK, 0xFFFFFFFFU);

			return Bits();
		}

		void Unit03_RegisterMem::Execute(byte code)
		{
			// 5 bits are valid
			switch(code & 0x1F)
			{
				// Stop
			case 0x00:
				CR &= 0x04;	// reset all but ME
				break;

				// Write new address
			case 0x01:
				PA = AD;
				AD = databus;	// AD is a byte, no need to mask other bits
				break;

				// Output current address
			case 0x02:
				CR &= 0x04;	// reset all but ME
				CR |= 0x08; // set AO
				break;

				// Output addressed data with mask
			case 0x03:
				CR = 0x06;	// set ME and DO
				break;

				// write data from the data bus (with mask)
			case 0x04:
				*(mem+AD) = Bits(databus, MK).Replace(*(mem+AD));
				break;

				// write data from the data bus with mask and increment
			case 0x05:
				*(mem+AD) = Bits(databus, MK).Replace(*(mem+AD));
				AD++;
				break;

				// write data from the data bus with mask and decrement
			case 0x06:
				*(mem+AD) = Bits(databus, MK).Replace(*(mem+AD));
				AD--;
				break;

				// next element
			case 0x07:
				AD++;
				break;

				// previous element
			case 0x08:
				AD--;
				break;

				// write mask from the databus
			case 0x09:
				MK = databus;
				break;

				//output current mask
			case 0x0A:
				CR &= 0x04;	// reset everything but ME
				CR |= 0x01;	// set MO

				// enable mask
			case 0x0B:
				CR |= 0x04; // set ME
				break;

				// disable mask
			case 0x0C:
				CR &= 0xFB; // reset ME
				break;

				// output data without mask
			case 0x0D:
				CR = 0x02;	// set only DO, reset everything else
				break;

				// write data without mask
			case 0x0E:
				*(mem+AD) = databus;
				break;

				// write data without mask and increment
			case 0x0F:
				*(mem+AD++) = databus;
				break;

				// write data without mask and decrement
			case 0x10:
				*(mem+AD--) = databus;
				break;

				// restore address
			case 0x11:
				AD = PA;
				break;
			}
		}

		void Unit03_RegisterMem::Reset(bool hard)
		{
			// set default values
			AD = PA = CR = 0;
			MK = 0xFFFFFFFFU;

			if(hard)
				for(int i = 0; i < 256; i++)
					*(mem+i) = 0x00U;
		}
	}
}