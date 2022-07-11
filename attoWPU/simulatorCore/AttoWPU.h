#pragma once
#include "Units.h"
#include <iostream>
#include <string>
#include "AudioBuffer.h"

/*
	This class specifies the AttoWPU object, which represents the
	Weird Processing Unit itself and controls whole execution
	of the virtual WPU.
*/

namespace attoWPU
{
	using namespace shared;

	namespace simulator
	{
		class AttoWPU
		{
			// the whole bus
			unsigned long long busout, bus;
			Unit *units[256];	// holds all units of the attoWPU
			int lastins; // number of last executed instruction

			void Initialize(byte *attomemory, int attomem_size, bool attomem_free, unsigned long long *realtime);

			unsigned long long *timesource, time;

			// audio output
			AudioBuffer *speakerbuf;

			// stats
			unsigned long long cycles;

		public:
			AttoWPU();
			AttoWPU(byte *attomemory, int attomem_size, unsigned long long *realtime);

			void Cycle();	// single cycle
			//void Cycle(unsigned int cycles);	// run several cycles

			// Interfacing with the outside
			unsigned long long GetCycles() { return cycles; }
			unsigned long long GetBus() { return bus; }
			std::string GetBusStr();
			unsigned int GetAddr() { return (byte)(bus >> 56); }
			unsigned int GetCtrl() { return (bus >> 48) & 0xFFU; }
			unsigned int GetData() { return (bus >> 16) & 0xFFFFFFFFU; }
			unsigned int GetAjmp() { return bus & 0xFFFFU; }
			unsigned int GetIA() { return lastins; }

			void Reset(bool hard);

			// getting units related data
			unsigned int GetLEDrow(int id) { return ((Unit0A_LED *)units[10])->GetRow(id); }
			void GetTextDisplay(char *copyto) { ((Unit0B_Text*)units[11])->GetText(copyto); }	// copyto must be at least 164 bytes long

			byte *GetRegMemPtr() { return ((Unit03_RegisterMem*)units[3])->GetMemPtr(); }
			byte *GetSQueuePtr() { return ((Unit09_SmallQueue*)units[9])->GetMemPtr(); }
			byte *GetTextDispPtr() { return ((Unit0B_Text*)units[11])->GetMemPtr(); }
			byte *GetLCDDispPtr() { return ((Unit0C_LCD*)units[12])->GetMemPtr(); }

			unsigned int GetSpeakerDataSize() { return speakerbuf->GetDataSize(); }
			unsigned int GetSpeakerSampleRate() { return speakerbuf->GetSampleRate(); }
			const byte *GetSpeakerSoundData() { return speakerbuf->GetData(); }
			void ResetSpeakerBuffer() { speakerbuf->Reset(); }

			// copyto mus be at least 49152 bytes
			void GetLCDFullData24b(byte *copyto) { ((Unit0C_LCD*)(units[12]))->GetFullData24b(copyto); }
			void GetLCDNewData24b(byte *copyto) { ((Unit0C_LCD*)(units[12]))->GetNewData24b(copyto); }
			void LCDResetUpdate() { ((Unit0C_LCD*)(units[12]))->ResetUpdate(); }
			bool LCDNewData() { return ((Unit0C_LCD*)(units[12]))->NewData(); }

			void SetInputNumKey(unsigned int id, bool status) { ((Unit0D_InputCtrl*)(units[13]))->SetNumKey(id, status); }
			void SetInputAlphaKey(unsigned int id, bool status) { ((Unit0D_InputCtrl*)(units[13]))->SetAlphaKey(id, status); }
			void SetInputSwitches(unsigned int row, unsigned int data) { ((Unit0D_InputCtrl*)(units[13]))->SetSwitches(row, data); }
			void SetInputSwitch(unsigned int row, unsigned int num, bool status) { ((Unit0D_InputCtrl*)(units[13]))->SetSwitch(row, num, status); }

			
		};
	}
}
