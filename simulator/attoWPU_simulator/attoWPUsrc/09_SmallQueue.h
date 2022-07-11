#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit09_SmallQueue : public Unit
		{
			// object functionality
			unsigned int databus;

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			byte *mem;	// pointer to the memory

			// internal registers
			byte AD, CR;
			unsigned int MK, CD, FL;

			// detect bits
			bool AO() { return (CR&0x01) != 0; }
			bool DO() { return (CR&0x02) != 0; }
			bool DW() { return (CR&0x04) != 0; }
			bool ME() { return (CR&0x08) != 0; }
			bool QR() { return (CR&0x80) != 0; }
			
		public:
			Unit09_SmallQueue();
			~Unit09_SmallQueue();

			virtual Bits ReadData();
			virtual void Reset(bool hard);

			byte *GetMemPtr() { return mem; }	// get direct accesss to the memory DANGEROUS
		};
	}
}