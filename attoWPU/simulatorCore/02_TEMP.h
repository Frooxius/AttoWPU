#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit02_TEMP : public Unit
		{
			// object functionality
			unsigned int databus;

			// internal registers
			unsigned int DT, MK;
			byte CR;

			// detect bits
			bool MO() { return (CR&0x01) != 0; }
			bool DO() { return (CR&0x02) != 0; }
			bool ME() { return (CR&0x04) != 0; }

		public:
			Unit02_TEMP();

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);
			virtual Bits ReadData();
			virtual void Reset(bool hard);

			// get data
			unsigned int GetDT() { return DT; }

		};
	}
}