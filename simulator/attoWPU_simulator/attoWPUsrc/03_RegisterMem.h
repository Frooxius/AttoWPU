#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit03_RegisterMem : public Unit
		{
			// object functionality
			unsigned int databus;
			unsigned int *mem;	// pointer to data in the memory

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			// Internal registers
			byte AD, PA, CR;
			unsigned int MK;

			//	Detect bits
			bool MO() { return (CR&0x01) != 0; }
			bool DO() { return (CR&0x02) != 0; }
			bool ME() { return (CR&0x04) != 0; }
			bool AO() { return (CR&0x08) != 0; }

		public:
			Unit03_RegisterMem();
			virtual ~Unit03_RegisterMem();
			virtual Bits ReadData();	
			virtual void Reset(bool hard);

			byte *GetMemPtr() { return reinterpret_cast<byte *>(mem); }	// get direct accesss to the memory DANGEROUS
		};
	}
}