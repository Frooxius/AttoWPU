#include "0B_Text.h"
#include <cctype>

namespace attoWPU
{
	namespace simulator
	{
		Unit0B_Text::Unit0B_Text() : Unit(0x0B)
		{
			mem = new byte[160];
			Reset(true);			
		}

		void Unit0B_Text::Execute(byte code)
		{
			switch(code&0x0F)
			{
				// stop data output
			case 0x00:
				CR = 0;
				break;

				// write address
			case 0x01:
				 AD = (byte)databus;
				 if(AD >= 160)
					 AD = 0;
				break;

				// write character
			case 0x02:
				*(mem+AD) = (byte)databus;
				break;

				// write character and move to the next
			case 0x03:
				*(mem+AD++) = (byte)databus;
				if(AD == 160)
					AD = 0;
				break;

				// next character
			case 0x04:
				AD++;
				if(AD == 160)
					AD = 0;
				break;

				// previous
			case 0x05:
				AD--;
				if(AD > 159)
					AD = 159;
				break;

				// output address
			case 0x06:
				CR = 0x01;
				break;

				// output character
			case 0x07:
				CR = 0x02;
				break;

				// reset cursor
			case 0x08:
				AD = 0;
				break;

				// clear memory
			case 0x09:
				AD = 0;
				for(int i = 0; i < 160; i++)
					*(mem+i) = 0;
				break;
			}
		}

		Bits Unit0B_Text::ReadData()
		{
			if(AO())
				return Bits(AD, 0x000000FFU);
			if(DO())
			{
				if(AD < 160)
					return Bits(*(mem+AD), 0x000000FFU);
				else
					return Bits(0, 0x000000FFU);
			}

			return Bits();
		}

		void Unit0B_Text::GetText(char *copyto)
		{
			for(int i = 0; i < 160; i++)
			{
				if(std::isprint(*(mem+i)))
					*(copyto++) = *(mem+i);
				else
					*(copyto++) = ' ';
				if( i == 39 || i == 79 || i == 119 )
					*(copyto++) = '\n';
			}

			*copyto = '\0';
		}

		void Unit0B_Text::Reset(bool hard)
		{
			AD = CR = 0;
			// inicialize the memory
			if(hard)
				for(int i = 0; i < 160; ++i)
					*(mem+i) = 0x00;
		}
	}
}