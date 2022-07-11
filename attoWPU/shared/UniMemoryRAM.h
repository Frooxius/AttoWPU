#pragma once
#include "UniMemory.h"

namespace attoWPU
{
	namespace shared
	{
		class UniMemoryRAM : public UniMemory
		{
			byte *mem;	// pointer to the data
			virtual byte _read(unsigned long long addr);
			virtual void _write(unsigned long long addr, byte data);

			void Allocate(unsigned long long size);

		public:
			UniMemoryRAM(unsigned long long size, bool readonly = false);
			UniMemoryRAM(unsigned long long size, byte bits, bool readonly = false);

			virtual ~UniMemoryRAM();
		};
	}
}