#pragma once
#include "Unit.h"
#include "05_OUT.h"
#include "02_TEMP.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit04_ALU : public Unit
		{
			// object functionality
			unsigned int databus;
			Unit05_OUT *out;	// out register
			Unit02_TEMP *temp;	// temp register

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp) { databus = data; }
			virtual void Execute(byte code);
			
		public:
			Unit04_ALU(Unit05_OUT *outRegister, Unit02_TEMP *tempRegister);

			// this unit doesn't output any data
			virtual Bits ReadData() { return Bits(); }
			virtual void Reset(bool hard) { databus = 0; }
		};
	}
}