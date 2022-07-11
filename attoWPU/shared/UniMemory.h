#pragma once
#include "Bits.h"

/*
	This abstract class provides read and write operations for a memory,
	where the memory itself is implemented by derived classes.	
*/

namespace attoWPU
{
	namespace shared
	{
		class UniMemory
		{
			// Derived classes must implement these two
			virtual byte _read(unsigned long long addr) = 0;
			virtual void _write(unsigned long long addr, byte data) = 0;

			unsigned long long	address,		// Currently addressed location
								address_mask,	// Address for the mask to remove invalid bits
								mem_size;	// Maximum address (size)

			bool rom;	// readonly memory

			inline unsigned long long FilterAddress(unsigned long long addr);

		protected:
			void Init(byte bits, unsigned long long size, bool readonly);

		public:
			UniMemory();
			UniMemory(byte bits, unsigned long long size, bool readonly);

			virtual ~UniMemory() { };
			
			byte Read();					// read from current address and move to the next one
			byte Read(unsigned long long addr);	// read from given address
			void Write(byte data);		// write to current address and move to the next one
			void Write(byte data, unsigned long long addr);	// write to given address

			void Next() { address++; }	// next address

			bool End() { return address >= mem_size; }	// returns if end of the memory was reached
			unsigned long long Size() { return mem_size; }	// get the size of the memory

			
			// overloaded operators for easier access
			byte operator*();	// read value without moving to the next one
			void operator++() { Next(); }	// can be only used as standalone, doesn't return reference
			void operator++(int dummy) { Next(); }
			UniMemory &operator=(unsigned long long addr) { address = addr; }
		};
	}
}