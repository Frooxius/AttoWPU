#include "AttocodeWriteMem.h"
#include <fstream>
#include "Exception.h"

namespace attoWPU
{
	namespace assembler
	{
		void AttocodeWriteMem::__write(byte data)
		{
			*(mem+address) = data;
		}

		void AttocodeWriteMem::__address(int addr)
		{
			address = addr;
		}

		AttocodeWriteMem::AttocodeWriteMem(int _size)
		{
			size = _size;
			mem = new byte[size];
			free_mem = true;
		}

		AttocodeWriteMem::AttocodeWriteMem(int _size, byte *memory)
		{
			size = _size;
			mem = memory;
			free_mem = false;
		}

		AttocodeWriteMem::~AttocodeWriteMem()
		{
			if(free_mem)
				delete [] mem;
		}

		byte *AttocodeWriteMem::RetrieveData()
		{
			free_mem = false;
			return mem;
		}

		void AttocodeWriteMem::SaveToFile(char *filename)
		{
			std::ofstream file(filename, std::ios::out | std::ios::binary);
			if(!file)
				throw Exception("Cannot write to a disk.");

			file.write((char *)mem, topaddress);
			file.close();
		}
	}
}