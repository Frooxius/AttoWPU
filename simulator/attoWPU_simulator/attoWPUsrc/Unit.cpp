#include "Unit.h"

namespace attoWPU
{
	namespace simulator
	{
		void Unit::Initialize()
		{
			executionBit = false;
		}

		Unit::Unit(byte addr)
		{
			address = addr;
			Initialize();
		}

		void Unit::Update(unsigned long long busdata)
		{
			// Extract individual values

			byte addr = busdata >> 56;
			byte ctrl = (busdata >> 48) & 0xFFU;
			unsigned int data = (busdata >> 16) & 0xFFFFFFFFU;
			unsigned int ajmp = busdata & 0xFFFFU;

			// Detect execution bit
			if(!executionBit && (ctrl & 0x01) && addr == address)
			{
				this->Execute( (ctrl & 0xFE) >> 1);
				executionBit = true;
			}

			if((executionBit && !(ctrl & 0x01)) || addr != address)
				executionBit = false;			

			// Update the unit itself
			this->UpdateUnit(data, addr, ctrl, ajmp);
		}
	}
}