#include "AttocodeWrite.h"
#include "Exception.h"
#include <string>

namespace attoWPU
{
	namespace assembler
	{
		AttocodeWrite::AttocodeWrite()
		{
			address = topaddress = size = 0;
		}

		int AttocodeWrite::GetSize()
		{
			return size;
		}

		byte AttocodeWrite::Opcode(AttoInstruction i, int bit)
		{
			if(bit > 63 || bit < 0)
			{
				std::string msg = "Can't create opcode for bit ";
				msg += bit;
				throw Exception(msg.c_str(), true);
			}

			return (i << 6) | bit;	// create the attoinstruction opcode
		}

		void AttocodeWrite::Write(byte data)
		{
			if(address >= size)
				throw Exception("Trying to write to a full attocode storage memory.", true);

			__write(data);
			if(address++ == topaddress)
				topaddress++;
		}


		void AttocodeWrite::Write(attoWPU::assembler::AttoInstruction i, int bit)
		{
			Write(Opcode(i, bit));
		}

		void AttocodeWrite::SetAddress(int addr)
		{
			if((addr < 0) | (addr > topaddress))
				throw Exception("Trying to address invalid location in the attocodo storage memory.", true);

			__address(addr);
		}
	}
}