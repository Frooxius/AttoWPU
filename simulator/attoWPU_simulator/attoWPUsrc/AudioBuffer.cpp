#include "AudioBuffer.h"

namespace attoWPU
{
	namespace shared
	{
		AudioBuffer::AudioBuffer(unsigned int samplerate, unsigned int size)
		{
			_size = size;
			_data = new byte[_size];
			rate = samplerate;
			Reset();
		}

		void AudioBuffer::Reset()
		{
			pos = 0;
		}

		unsigned long long AudioBuffer::Write(byte data)
		{
			if(!Full())
				*(_data+pos++) = data;

			return pos*(1000000/rate);
		}
	}
}