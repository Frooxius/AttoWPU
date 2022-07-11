#pragma once
#include "Unit.h"
#include "00_aPC.h"

namespace attoWPU
{
	namespace simulator
	{
		class UnitS0_aPCWrite : public Unit
		{
			Unit00_aPC *_aPC;	// new address will be written to this aPC			
			bool jumpbit;

			virtual void Execute(byte code) {};
			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			
		public:
			//UnitS0_aPCWrite() { _aPC = 0; }
			UnitS0_aPCWrite(Unit00_aPC *aPC) : Unit(0xFF) { _aPC = aPC; jumpbit = false; };
						
			virtual Bits ReadData() { return Bits(); };
			virtual void Reset(bool hard) { }
		};
	}
}