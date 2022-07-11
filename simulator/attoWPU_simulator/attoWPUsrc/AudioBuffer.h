#pragma once
#include "Bits.h"

namespace attoWPU
{
	namespace shared
	{
		class AudioBuffer
		{
			unsigned int _size, pos, rate;
			byte *_data;

		public:
			AudioBuffer(unsigned int samplerate = 44100, unsigned int size = 32768);

			void Reset();
			// writes the sample and then returns current position in ns
			unsigned long long Write(byte sample);
			bool Full() { return pos == _size; }

			unsigned int GetDataSize() { return pos; }
			unsigned int GetSampleRate() { return rate; }
			const byte *GetData() { return _data; }
		};
	}
}