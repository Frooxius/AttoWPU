#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit00_aPC : public Unit
		{
			// object functionality
			unsigned int databus;	// value from the data bus

			void Initialize();

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			// Internal registers
			int IA;		// instruction address
			byte CR;	// control register

			// Detect bits
			bool AO() { return (CR & 0x01) != 0; }	// address output
			bool SP() { return (CR & 0x02) != 0; } // skip pulse

		public:
			Unit00_aPC();

			int GetIA() { return IA; }	// return instruction address
			void SetIA(unsigned int newIA) { IA = 0x00FFFFFFU & newIA; CR |= 0x02; };
			void Pulse();	// increment IA

			virtual void Reset(bool hard);
			virtual Bits ReadData();
		};
	}
}