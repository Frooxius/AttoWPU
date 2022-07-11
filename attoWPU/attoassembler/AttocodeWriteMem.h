#pragma once
#include "AttocodeWrite.h"

/*
	This class stores the attocode in a memory.
*/

namespace attoWPU
{
	namespace assembler
	{
		class AttocodeWriteMem : public AttocodeWrite
		{
		private:
			byte *mem;		// pointer to a memory
			bool free_mem;	// determines whether to free memory after object is destroyed or not

		protected:
			virtual void __write(byte data);
			virtual void __address(int addr);

		public:
			AttocodeWriteMem(int _size);
			AttocodeWriteMem(int _size, byte *memory);
			~AttocodeWriteMem();

			void SaveToFile(char *filename);	// save everything to a file
			byte *RetrieveData();				// get pointer to the data, this will also prevent them from being freed!
		};
	}
}