#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit0B_Text : public Unit
		{
			// object functionality
			unsigned int databus;

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
				{ databus = data; }
			virtual void Execute(byte code);

			byte *mem;	// character memory
			
			// internal registers
			byte AD, CR;			

			// detect bits
			bool AO() { return (CR&0x01) != 0; }
			bool DO() { return (CR&0x02) != 0; }

		public:
			Unit0B_Text();
			~Unit0B_Text() { delete[] mem; }

			virtual Bits ReadData();
			virtual void Reset(bool hard);

			// interfacing with the simulator
			void GetText(char *copyto);	// copyto must be at least 164 bytes long
			byte *GetMemPtr() { return mem; }	// get direct accesss to the memory DANGEROUS
		};
	}
}