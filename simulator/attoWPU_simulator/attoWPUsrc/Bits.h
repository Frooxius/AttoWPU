#pragma once

// This struct is used to transfer read/write values from/to the bus with specification of valid bits

namespace attoWPU
{
	namespace shared
	{
		// Define byte
		typedef unsigned char byte;

		struct Bits
		{
			unsigned int val,	// value itself
						 mask;	// mask determining valid bits
			Bits(unsigned int v, unsigned int m) { val = v; mask = m; }
			Bits() { val = 0; mask = 0; }	// defeault constructor creates empty data (mask has only zeroes)

			// replace values in v with bits from val using the mask
			unsigned int Replace(unsigned int v)
			{
				return (v&(~mask)) | (val&mask);
			}
		};
	}
}