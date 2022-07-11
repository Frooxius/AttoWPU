#include "UniMemoryRAM.h"

namespace attoWPU
{
	namespace shared
	{

		byte UniMemoryRAM::_read(unsigned long long addr)
		{
			return *(mem+addr);
		}

		void UniMemoryRAM::_write(unsigned long long addr, byte data)
		{
			*(mem+addr) = data;
		}

		void UniMemoryRAM::Allocate(unsigned long long size)
		{
			mem = new byte[(unsigned int)size];
		}

		UniMemoryRAM::UniMemoryRAM(unsigned long long size, bool readonly)
		{
			// calculate valid bits
			int bits = 1;
			while( size > (unsigned long long)(0x01 << bits++));
			Init(bits, size, readonly);
			Allocate(size);
		}

		UniMemoryRAM::UniMemoryRAM(unsigned long long size, byte bits, bool readonly)
			: UniMemory(bits, size, readonly)
		{
			Allocate(size);
		}

		UniMemoryRAM::~UniMemoryRAM()
		{
			delete[] mem;
		}
	}
}