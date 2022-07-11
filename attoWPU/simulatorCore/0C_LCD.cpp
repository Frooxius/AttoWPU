#include "0C_LCD.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit0C_LCD::Unit0C_LCD() : Unit(0x0C)
		{
			mem = new byte[49152]; // 128*128*3
			mem2 = new byte[49152];
			Reset(true);
		}

		void Unit0C_LCD::Reset(bool hard)
		{
			AD = CR = 0;
			minaddr = 0;
			maxaddr = 49151;

			if(hard)
				for(int i = 0; i < 49152; ++i)
				{
					*(mem+i) = 0;
					*(mem2+i) = 0xFF;
				}
		}

		void Unit0C_LCD::WritePixel(int address, unsigned int data)
		{
			byte *writemem;
			if(!BE() || !BM())
				writemem = mem;
			else
				writemem = mem2;
			
			// check if address is in the range
			if(address > 16383)
				return;
			address *= 3;	// adjust for the actual data (pixel is 3 bytes)

			// update min and max
			if(minaddr > address)
				minaddr = address;
			if(maxaddr < address)
				maxaddr = address;

			*(writemem+address+2) = data;
			*(writemem+address+1) = data >> 8;
			*(writemem+address) = data >> 16;
		}

		unsigned int Unit0C_LCD::ReadPixel(int address)
		{
			byte *writemem;
			if(!BE() || !BM())
				writemem = mem;
			else
				writemem = mem2;

			// check if address is in the range
			if(address > 16383)
				return 0;
			address *= 3;	// adjust for the actual data (pixel is 3 bytes)

			return (unsigned int)*(writemem+address+2)
			| ((unsigned int)*(writemem+address+1)) << 8
			| ((unsigned int)*(writemem+address)) << 16;			
		}


		void Unit0C_LCD::Execute(byte code)
		{
			switch(code&0x0F)
			{
				// stop data output
			case 0x00:
				CR &= 0x0C;
				break;

				// write address
			case 0x01:
				AD = databus & 0x0000FFFFU;
				if(AD > 16383)
					AD = 0;
				break;

				// write pixel
			case 0x02:
				WritePixel(AD, databus & 0x00FFFFFF);
				break;

				// write character and move to the next
			case 0x03:
				WritePixel(AD, databus & 0x00FFFFFF);
				if(++AD > 16383)
					AD = 0;
				break;

				// next pixel
			case 0x04:
				if(++AD > 16383)
					AD = 0;
				break;

				// previous
			case 0x05:
				if(--AD < 0)
					AD = 16383;
				break;

				// output address
			case 0x06:
				CR |= 0x01;
				CR &= 0xFD;
				break;

				// output pixel
			case 0x07:
				CR |= 0x02;
				CR &= 0xFE;
				break;

				// reset cursor
			case 0x08:
				AD = 0;
				break;

				// clear memory
			case 0x09:
				AD = 0;

				minaddr = 0;
				maxaddr = 49151;

				byte *writemem;
				if(!BE() || !BM())
					writemem = mem;
				else
					writemem = mem2;

				for(int i = 0; i < 49152; i++)
					*(writemem+i) = 0;
				break;

				// single buffer
			case 0x0A:
				CR &= 0xFB;
				minaddr = 0;
				maxaddr = 49151;
				break;

				// double buffer
			case 0x0B:
				CR |= 0x04;
				minaddr = 0;
				maxaddr = 49151;
				break;

				// buffer switch
			case 0x0C:
				CR ^= 0x08;
				minaddr = 0;
				maxaddr = 49151;
				break;
			}
		}

		Bits Unit0C_LCD::ReadData()
		{
			if(AO())
				return Bits(AD, 0x0000FFFFU);
			if(DO())
			{
				if(AD < 49152)
					return Bits(ReadPixel(AD), 0x00FFFFFFU);
				else
					return Bits(0, 0x000000FFU);
			}

			return Bits();
		}

		void Unit0C_LCD::CopyTo(byte *copyto, int min, int max)
		{
			byte *readmem;
			if(!BE())
				readmem = mem;
			else
			{
				if(BM())
					readmem = mem;
				else
					readmem = mem2;
			}

			for(int i = min; i < max; i++)
				*(copyto+i) = *(readmem+i);
		}


	}
}