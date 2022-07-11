#pragma once
#include "Bits.h"
#include "DataRead.h"
#include "DataWrite.h"

/*
	This abstract class specifies general features of the various units of the attoWPU.
	From this class, many others are derived, that implement Unit-specific features.
*/

namespace attoWPU
{
	using namespace shared;

	namespace simulator
	{
		class Unit
		{
			bool executionBit;	// last known status of the execution bit
			byte address;	// address of the device
			void Initialize();	// initialize itself

		protected:
			virtual void UpdateUnit(unsigned int data, byte addr, byte ctrl, int ajmp) = 0;	// update method, that has to be implemented by the derived class
			virtual void Execute(byte code) = 0;	// execute a function with given code

		public:
			Unit(byte addr);		// constructor
			//Unit(byte addr, DataRead dump);	// initialize using dumped data
			virtual ~Unit() { };	// destructor has to be virtual too

			virtual Bits ReadData() = 0;
			virtual void Reset(bool hard) = 0;
			void Update(unsigned long long busdata);	// updates the status of the device

			// Dumping and exporting TO DO
			//virtual void Dump(DataWrite dumpg);
		};
	}
}