#pragma once
#include "Unit.h"
#include "AudioBuffer.h"

namespace attoWPU
{
	namespace simulator
	{
		class Unit0F_SpeakerOut : public Unit
		{
			// object functionality
			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp);
			virtual void Execute(byte code);

			void WriteSoundData();	// write new sound data

			unsigned long long *time, // access to real time
								lasttime; // last time, when data were written to the buffer
			AudioBuffer *buffer;

			bool SPKR;	

		public:
			Unit0F_SpeakerOut(shared::AudioBuffer *buf, unsigned long long *realtime);

			virtual Bits ReadData() { return Bits(); }
			virtual void Reset(bool hard) { SPKR = false; }
		};
	}
}