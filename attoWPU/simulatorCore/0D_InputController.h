#pragma once
#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit0D_InputCtrl : public Unit
		{
			// object functionality
			unsigned int databus;

			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			// Holding status (pressed/not pressed) of all keys, both numeric and alphanumeric
			bool keystatus[37];
			// Status of switch rows
			unsigned int switches[4];

			unsigned int ReadNum(unsigned int skip);
			unsigned int ReadKey(unsigned int skip);
			
			// Internal registers
			unsigned int TD;	// temporary data
			byte CR;	// control register
			//bits
			bool DO() { return (CR&0x01) == 1; }	// data output
			bool BM() { return (CR&0x02) == 1; } // byte mode

		public:
			Unit0D_InputCtrl();

			virtual Bits ReadData();
			virtual void Reset(bool hard);

			// getting input
			void SetNumKey(unsigned int id, bool status);	// id = 0..9 - numbers, 10 - decimal point
			void SetAlphaKey(unsigned int id, bool status);	// id = lowercaseletter - 'a'.
			void SetSwitches(unsigned int row, unsigned int data);
			void SetSwitch(unsigned int row, unsigned int num, bool status);
		};
	}
}