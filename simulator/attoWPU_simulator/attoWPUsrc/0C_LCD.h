#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit0C_LCD : public Unit
		{
			// object functionality
			unsigned int databus;

			int minaddr, maxaddr;	// storing min and max AD since the last update
			void CopyTo(byte *copyto, int min = 0, int max = 49152);

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
				{ databus = data; }
			virtual void Execute(byte code);

			void WritePixel(int address, unsigned int data);
			unsigned int ReadPixel(int address);

			byte *mem, *mem2;	// pixel memory
			
			// internal registers
			int AD;
			byte CR;			

			// detect bits
			bool AO() { return (CR&0x01) != 0; }
			bool DO() { return (CR&0x02) != 0; }
			bool BE() { return (CR&0x04) != 0; }
			bool BM() { return (CR&0x08) != 0; }

		public:
			Unit0C_LCD();
			~Unit0C_LCD() { delete[] mem; }

			virtual Bits ReadData();
			virtual void Reset(bool hard);

			// interfacing with the simulator
			// copyto mus be at least 49152 bytes
			void GetFullData24b(byte *copyto) { CopyTo(copyto); }	// get all data
			void GetNewData24b(byte *copyto) { CopyTo(copyto, minaddr, maxaddr); }	// get only updated region
			void ResetUpdate() { minaddr = 49152; maxaddr = 0; };	// reset updated region
			bool NewData() { return (minaddr <= maxaddr); }
			byte *GetMemPtr() { return mem; }	// get direct accesss to the memory DANGEROUS
		};
	}
}