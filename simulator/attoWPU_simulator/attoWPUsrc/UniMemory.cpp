#include "UniMemory.h"

namespace attoWPU
{
	namespace shared
	{
		void UniMemory::Init(byte bits, unsigned long long size, bool readonly)
		{
			address = 0;
			mem_size = size;
			rom = readonly;
			// compute address mask
			address_mask = 0;
			while(bits--)
				address_mask |= 0x01ULL << bits;
		}

		UniMemory::UniMemory()
		{
			Init(0, 0, true);
		}

		UniMemory::UniMemory(byte bits, unsigned long long size, bool readonly)
		{
			Init(bits, size, readonly);
		}

		unsigned long long UniMemory::FilterAddress(unsigned long long addr)
		{
			unsigned long long masked_addr = addr&address_mask;
			return (masked_addr <= mem_size) ? masked_addr : mem_size-1;
		}

		byte UniMemory::Read()
		{
			return _read(FilterAddress(address++));
		}

		byte UniMemory::Read(unsigned long long addr)
		{
			return _read(FilterAddress(address = addr));
		}

		void UniMemory::Write(byte data)
		{
			if(rom) return;
			_write(FilterAddress(address++), data);
		}

		void UniMemory::Write(byte data, unsigned long long addr)
		{
			if(rom) return;
			_write(FilterAddress(address = addr), data);
		}

		byte UniMemory::operator*()
		{
			return _read(FilterAddress(address));
		}
	}
}