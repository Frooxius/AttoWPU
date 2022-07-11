#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit0A_LED : public Unit
		{
			// object functionality
			unsigned int databus;

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
				{ databus = data; }
			virtual void Execute(byte code)
				{ ROW[code&0x03] = databus; }
			
			// internal registers
			unsigned int ROW[4];

		public:
			Unit0A_LED() : Unit(0x0A) { Reset(false); }
			
			virtual Bits ReadData() { return Bits(); }
			virtual void Reset(bool hard) { databus = ROW[0] = ROW[1] = ROW[2] = ROW[3] = 0; }

			// For iterfacing with the simulator
			unsigned int GetRow(int id);
		};
	}
}