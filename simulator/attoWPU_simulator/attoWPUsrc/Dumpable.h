#pragma once
#include "Bits.h"
#include "UniMemory.h"

/*
	This abstract class specifies objects, that can be dumped:
	complete binary copy of their data can be saved and later 
	reloaded.

	All objects must implement abstract functions, while
	the rest is handled by the dumpable class.

	Objects must initialize dumpable class with list of variables
	and memory locations, that have to be saved, as well as version
	number, that's used to detect, if dump data are correct.
*/

namespace attoWPU
{
	namespace shared
	{
		class Dumpable
		{
			UniMemory *storage;
			unsigned int	dumpver;	// version of the dump data

			int ObjectID();

		protected:
			virtual void _dump() = 0;	// method to dump all the data form the object, must be implemented by derived classes
			virtual void _rest() = 0;	// method to restore all the data into the object

			// write dumped data, by default only a single value, by changing the amount variable, it's possible to write arrays
			template <typename X> void Write(unsigned int ver, X *data, unsigned int amount = 1)
			{
				byte *raw = reinterpret_cast<byte *>(*data);	// get access to data by bytes

				for(int i = 0; i < amount*sizeof(X); i++)		// process all data
					storage->Write(*(raw+i));
			}

			/*
				ver - version of the object, where was this value implemented
				def - default value, that will be used when the dump version is older t
			*/
			template <typename X> void Read(unsigned int ver, X *data, X def, unsigned int amount = 1)
			{
				if(ver <= dumpver)
				{
					byte *raw = reinterpret_cast<byte *>(*data);

					for(int i = 0; i < amount*sizeof(X); i++)
						*(raw+i) = storage->Read();
				}
				else
				{
					// when the dump is older, then there are no data for given data, so default value must be used
					while(amount--)
						*(data++) = def;
				}
			}

		public:
			Dumpable(unsigned int ver);

			void Dump(UniMemory *target);	// dump whole object
			void Rest(UniMemory *source);	// restore whole object
		};
	}
}