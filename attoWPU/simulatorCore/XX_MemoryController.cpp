#include "XX_MemoryController.h"

namespace attoWPU
{
	namespace simulator
	{
		UnitXX_MemoryController::UnitXX_MemoryController(UniMemory *memory, MemoryControllers unit, bool autofree)
			: Unit(unit)
		{
			mem = memory;
			free = autofree;
			SZ = mem->Size();	// get size
			Reset(false);
		}
		
		void UnitXX_MemoryController::Reset(bool hard)
		{
			AD = 0;
			CR = 0xC0;	// 32 cell size by default
			
			// TO DO hard
		}

		UnitXX_MemoryController::~UnitXX_MemoryController()
		{
			if(free)
				delete mem;
		}

		void UnitXX_MemoryController::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
		}		

		void UnitXX_MemoryController::Execute(byte code)
		{
			switch(code & 0x1F)
			{
				// stop data output
			case 0x00:
				CR &= 0xC0;
				break;

				// write lower address
			case 0x01:
				AD = (AD&0xFFFFFFFF00000000ULL) | databus;
				break;

				// output current lower address
			case 0x02:
				CR &= 0xC0;	// clear everything but CEL and CEH
				CR |= 0x01; // set ALO
				break;

				// output addressed data
			case 0x03:
				CR &= 0x0C0;
				CR |= 0x02;
				break;

				// write data from the data bus
			case 0x04:
				for(int i = 0; i <= CE(); i++)
					mem->Write(databus >> i*8, CalcAddr()+i);
				break;

				// write data from the databus and increment
			case 0x05:
				for(int i = 0; i <= CE(); i++)
					mem->Write(databus >> i*8, CalcAddr()+i);
				AD++;
				break;

				// write data from the data bus and decrement
			case 0x06:
				for(int i = 0; i <= CE(); i++)
					mem->Write(databus >> i*8, CalcAddr()+i);
				AD--;
				break;

				// next cell
			case 0x07:
				AD++;
				break;

				// previous cell
			case 0x08:
				AD--;
				break;

				// write higher address
			case 0x09:
				AD = (AD&0x00000000FFFFFFFFULL) |  ((unsigned long long)databus) << 32;
				break;

				// output higher address
			case 0x0A:
				CR &= 0xC0;	// clear everything but CEH and CEL
				CR |= 0x04; // set AHO
				break;

				// output memory capacity
			case 0x0B:
				CR &= 0xC0;
				CR |= 0x08;
				break;

				// set memory cell size
			case 0x0C:
				CR &= 0x3F;	// null the CEH and CEL
				CR |= databus << 6; // write new value to CEL and CEH
				break;
				
				// set memory cell size to 32b

			case 0x0D:
				CR &= 0x3F;
				CR |= 0xC0;
				break;

				// 24b
			case 0x0E:
				CR &= 0x3F;
				CR |= 0x80;
				break;

				// 16b
			case 0x0F:
				CR &= 0x3F;
				CR |= 0x40;
				break;

				// 8b
			case 0x10:
				CR &= 0x3F;
				break;

				// output memory cell size
			case 0x11:
				CR &= 0xC0;	// delete everything but CEL and CEH
				CR |= 0x10; // set the CO
				break;
			}
		}

		Bits UnitXX_MemoryController::ReadData()
		{
			if(ALO())
				return Bits((unsigned int)AD, 0xFFFFFFFFU);
			if(AHO())
				return Bits((unsigned int)(AD >> 32), 0xFFFFFFFFU);
			if(SO())
				return Bits(SZ, 0xFFFFFFFFU);
			if(CO())
				return Bits(CE(), 0xFFFFFFFFU);
			
			if(DO())
			{
				unsigned int data = 0;	// used to construct data
				for(int i = 0; i <= CE(); i++)
					data |= mem->Read(CalcAddr()+i) << i*8;
				return Bits(data, 0xFFFFFFFFU);
			}

			return Bits();
		}

	}
}