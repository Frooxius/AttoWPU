#include "0F_Speaker.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit0F_SpeakerOut::Unit0F_SpeakerOut(shared::AudioBuffer *buf, unsigned long long *realtime) : Unit(0x0F)
		{
			Reset(false);
			time = realtime;
			lasttime = *time;
			buffer = buf;
		}

		void Unit0F_SpeakerOut::UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp)
		{
			return;
			WriteSoundData();
		}

		void Unit0F_SpeakerOut::Execute(byte code)
		{
			return;
			SPKR = !SPKR;	// invert the output to the speaker
			WriteSoundData();
		}

		void Unit0F_SpeakerOut::WriteSoundData()
		{
			while(lasttime < *time)
			{
				lasttime += buffer->Write(0xFF*SPKR);
				if(buffer->Full()) // TO DO ERROR!
					lasttime = *time;
			}
		}
	}
}