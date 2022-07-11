#pragma once
#include "Values.h"

/*
	This abstract class is used to write attoinstruction opcodes to a unspecfied memory.
	The storage is implemented by derived classes.
*/

namespace attoWPU
{
	namespace assembler
	{
		typedef unsigned char byte;
		enum AttoInstruction {
			attoFall, attoRise, attoNot, attoHalt };

		class AttocodeWrite
		{
		private:
			byte Opcode(AttoInstruction i, int bit);	// create an attoinstruction

		protected:
			int address,	// current address
				topaddress,	// maximum address (in case previous address was addressed)
				size;		// maximum size
			virtual void __write(byte data) = 0;		// write data sequentially
			virtual void __address(int addr) = 0;	// seek different address

		public:
			AttocodeWrite();

			void Write(byte data);	// write raw data
			void Write(AttoInstruction i, int bit);
			void SetAddress(int addr);
			int GetAddress() { return address; }
			int GetTopAddress() { return topaddress; }

			int GetSize();
		};
	}
}