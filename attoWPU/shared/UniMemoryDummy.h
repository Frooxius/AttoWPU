#pragma once
#include "UniMemory.h"

namespace attoWPU
{
	namespace shared
	{
		class UniMemoryDummy : public UniMemory
		{
			virtual byte _read(unsigned long long addr) { return 0; }
			virtual void _write(unsigned long long addr, byte data) { }

		public:
			UniMemoryDummy() : UniMemory(0, 0, true) { };

			virtual ~UniMemoryDummy() { };
		};
	}
}