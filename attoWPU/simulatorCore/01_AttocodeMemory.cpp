#include "01_AttocodeMemory.h"

#include <cstdlib>

namespace attoWPU
{
	namespace simulator
	{
		Unit01_AttocodeMemory::Unit01_AttocodeMemory(byte *memory, unsigned int size, attoWPU::simulator::Unit00_aPC *aPC, bool autodelete) : Unit(0x01)
		{
			// check if it's illegal value
			if(size>0x01000000U || !size)
				size = 0x01000000U;

			// allocate memory
			_autodelete = autodelete;

			if(memory)
				mem = memory;
			else
			{
				mem = new byte[size];
				_autodelete = true;
			}
			

			// save important values
			_size = size;
			_aPC = aPC;

			Reset(!memory);
		}

		Unit01_AttocodeMemory::~Unit01_AttocodeMemory()
		{
			// cleanup
			if(_autodelete)
				delete[] mem;
		}

		byte Unit01_AttocodeMemory::Read(unsigned int addr)
		{
			// check if address exceeds memory size
			if(addr >= _size)
				return 0;
			return *(mem+addr);
		}

		void Unit01_AttocodeMemory::Write(unsigned int addr, byte data)
		{
			// check if address exceeds memory size
			if(addr >= _size)
				return;
			*(mem+addr) = data;
		}

		byte Unit01_AttocodeMemory::Attoins()
		{
			return Read(_aPC->GetIA());
		}

		void Unit01_AttocodeMemory::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			databus = data;
		}

		void Unit01_AttocodeMemory::Execute(byte code)
		{
			// remove invalid bits - 4 valid bits
			switch(code & 0x0F)
			{
				// Stop
			case 0x00:
				// null both AO and DO
				CR  = 0;
				break;

				// write address
			case 0x01:
				AD = databus & 0x00FFFFFF;
				break;

				// output address
			case 0x02:
				// null DO and set AO
				CR = 0x01;
				break;

				// output data
			case 0x03:
				// null AO and set DO
				CR = 0x02;
				break;

				// Write data
			case 0x04:
				Write(AD, databus);
				break;

				// Write data and increase
			case 0x05:
				Write(AD++, databus);
				if(AD > 0x00FFFFFF)
					AD = 0x00FFFFFF;
				break;

				// Write data and decrease
			case 0x06:
				Write(AD--, databus);
				if(AD < 0)
					AD = 0;
				break;

				// increase
			case 0x07:
				if(++AD > 0x00FFFFFF)
					AD = 0x00FFFFFF;
				break;

				// decrease
			case 0x08:
				if(--AD < 0)
					AD = 0;
				break;
			}
		}

		Bits Unit01_AttocodeMemory::ReadData()
		{
			if(AO())
				return Bits(AD, 0x00FFFFFF);
			if(DO())
				return Bits(Read(AD), 0x000000FF);
			return Bits();
		}

		void Unit01_AttocodeMemory::Reset(bool hard)
		{
			// set default values
			databus = AD = CR = 0;

			if(hard)
				for(int i = 0; i < _size; ++i)
					*(mem+i) = 0x00;
		}

	}
}