#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit05_OUT : public Unit
		{
			// internal registers
			unsigned int DT;	// data
			bool DO;	// data output bit

			virtual void Execute(byte code) { DO = code&0x01; }
			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp) {};

		public:
			Unit05_OUT() : Unit(0x05) { Reset(false); }

			virtual Bits ReadData();
			virtual void Reset(bool hard) { DT = DO = 0; }
			void Set(unsigned int value) { DT = value; }
		};
	}
}