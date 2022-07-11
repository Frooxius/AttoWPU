#pragma once
#include "Unit.h"
#include "00_aPC.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit01_AttocodeMemory : public Unit
		{
			// Functional members
			int databus;	// data from the data bus
			Unit00_aPC *_aPC;	// the atto program counter (so it can get the currently addressed attoinstruction)
			unsigned int _size;	// size of the memory
			byte *mem;	// pointer to memory itself
			bool _autodelete;	// free memory when the object is destroyed?

			byte Read(unsigned int addr);	// read a byte from addressed position
			void Write(unsigned int addr, byte data);	// write a byte

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			// Internal registers
			int AD;		// address
			byte CR;	// control register

			// Detect bits
			bool AO() { return (CR & 0x01) != 0; }
			bool DO() { return (CR & 0x02) != 0; }

		public:
			~Unit01_AttocodeMemory(); // destructor

			// if memory == zero it will be allocated automatically
			Unit01_AttocodeMemory(byte *memory, unsigned int size, Unit00_aPC *aPC, bool autodelete = true);

			byte Attoins();		// get currently pointed attoinstruction
			virtual Bits ReadData();
			virtual void Reset(bool hard);
		};
	}
}