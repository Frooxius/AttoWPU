#include "AttoWPU.h"
#include "UniMemoryDummy.h"
#include "UniMemoryRAM.h"

namespace attoWPU
{
	namespace simulator
	{
		AttoWPU::AttoWPU()
		{
			Initialize(0, 16*1024*1024, true, 0);
		}

		AttoWPU::AttoWPU(byte *attomemory, int attomem_size, unsigned long long *realtime)
		{
			Initialize(attomemory, attomem_size, false, realtime);
		}

		void AttoWPU::Reset(bool hard)
		{
			bus = busout = lastins = 0;

			if(hard)
				cycles = 0;
			
			for(int i = 0; i < 15; i++)
				units[i]->Reset(hard);
		}

		void AttoWPU::Initialize(byte *attomemory, int attomem_size, bool attomem_free, unsigned long long *realtime)
		{
			// reset the bus
			bus = busout = 0;
			cycles = 0;
			lastins = 0;	

			// configure realtime
			timesource = realtime;
			time = 0;

			// audio output
			speakerbuf = new AudioBuffer();

			// create units
			units[0]	= new Unit00_aPC();
			units[1]	= new Unit01_AttocodeMemory(attomemory, attomem_size, (Unit00_aPC*)units[0], attomem_free);
			units[2]	= new Unit02_TEMP();
			units[3]	= new Unit03_RegisterMem();
			units[5]	= new Unit05_OUT();
			units[4]	= new Unit04_ALU( (Unit05_OUT*)units[5], (Unit02_TEMP*)units[2] );
			units[6]	= new Unit06_FPU( (Unit05_OUT*)units[5], (Unit02_TEMP*)units[2] );
			units[7]	= new UnitXX_MemoryController( new UniMemoryRAM(0x10000), MemoryControllerA );
			units[8]	= new UnitXX_MemoryController( new UniMemoryRAM(0x10000), MemoryControllerB );
			units[9]	= new Unit09_SmallQueue();
			units[10]	= new Unit0A_LED();
			units[11]	= new Unit0B_Text();
			units[12]	= new Unit0C_LCD();
			units[13]	= new Unit0D_InputCtrl();
			units[14]	= new Unit00_aPC();
			units[15]	= new Unit0F_SpeakerOut(speakerbuf, &time);

			units[16] = new UnitS0_aPCWrite((Unit00_aPC*)units[0]);
		}

		void AttoWPU::Cycle()
		{
			// update stats
			cycles++;

			if(timesource == 0)
				time = cycles*10;
			else
				time = *timesource;

			// increment IA
			((Unit00_aPC*)units[0])->Pulse();

			lastins = ((Unit00_aPC*)units[0])->GetIA();

			// get the current attoinstruction
			byte attoins = ((Unit01_AttocodeMemory*)units[1])->Attoins();
			
			// process attoinstruction
			switch(attoins & 0xC0)
			{
				// fall
				case 0x00:
					busout &= (0x7FFFFFFFFFFFFFFFULL >> (attoins & 0x3F)) | (0xFFFFFFFFFFFFFFFFULL << (64-(attoins & 0x3F)) );
					break;
				// rise
				case 0x40:
					busout |= 0x8000000000000000ULL >> (attoins & 0x3F);
					break;
				case 0x80:
					busout ^= 0x8000000000000000ULL >> (attoins & 0x3F);
					break;
				case 0xC0:
					// halt
					break;
			}

			// read and merge output from all devices
			bus = busout;
			for(int i = 0; i < 17; i++)
			{
				Bits readtemp = units[i]->ReadData();
				bus &= (((unsigned long long)(readtemp.val | ~readtemp.mask)) << 16) | 0xFFFF00000000FFFFULL;
			}
	
			// update units
			for(int i = 0; i < 17; i++)
				units[i]->Update(bus);

			bus =  busout;
			for(int i = 0; i < 17; i++)
			{
				Bits readtemp = units[i]->ReadData();
				bus &= (((unsigned long long)(readtemp.val | ~readtemp.mask)) << 16) | 0xFFFF00000000FFFFULL;
			}


		}

		std::string AttoWPU::GetBusStr()
			{
				std::string str;
				unsigned long long tmp = bus;
				for(int i = 0; i < 64; i++)
				{
					if(tmp & 0x8000000000000000ULL)
						str += "1";
					else
						str += "0";
					tmp <<= 1;
				}
				return str;
			}
	}
}