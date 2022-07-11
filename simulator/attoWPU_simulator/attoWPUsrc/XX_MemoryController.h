#pragma once
#include "Unit.h"
#include "UniMemory.h"

namespace attoWPU
{
	using namespace shared;

	namespace simulator
	{
		enum MemoryControllers {
			MemoryControllerA = 0x07,
			MemoryControllerB = 0x08
		};

		class UnitXX_MemoryController : public Unit
		{
			// object funcitonality
			unsigned int databus;
			UniMemory *mem;
			bool free;	// whether to free memory when the object is delted or not

			inline unsigned long long CalcAddr() { return AD*(CE()+1); }

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			// Internal registers
			unsigned long long AD,
				SZ;
			byte CR;

			// detect bits
			bool ALO() { return (CR&0x01) != 0; }
			bool DO() { return (CR&0x02) != 0; }
			bool AHO() { return (CR&0x04) != 0; }
			bool SO() { return (CR&0x08) != 0; }
			bool CO() { return (CR&0x10) != 0; }
			bool CEL() { return (CR&0x40) != 0; }
			bool CEH() { return (CR&0x80) != 0; }
			byte CE() { return CEL()+(CEH()<<1); }	

		public:
			UnitXX_MemoryController(UniMemory *memory, MemoryControllers unit, bool autofree = true);
			~UnitXX_MemoryController();

			virtual Bits ReadData();	
			virtual void Reset(bool hard);
		};
	}
}