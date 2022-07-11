#include "04_ALU.h"
#include "Values.h"

namespace attoWPU
{
	namespace simulator
	{
		Unit04_ALU::Unit04_ALU(Unit05_OUT *outRegister, Unit02_TEMP *tempRegister) : Unit(0x04)
		{
			databus = 0;
			out = outRegister;
			temp = tempRegister;
		}

		void Unit04_ALU::Execute(byte code)
		{

			switch(code & 0x3F)
			{
				// zero
			case 0x00:
				out->Set(0);
				break;

				// ADD
			case 0x01:
				out->Set(databus+temp->GetDT());
				break;

				// SUB
			case 0x02:
				out->Set(databus-temp->GetDT());
				break;

				// MULL
			case 0x03:
				out->Set( ((unsigned long long)databus * (unsigned long long)temp->GetDT()) );
				break;

				// MULH
			case 0x04:
				out->Set( ((unsigned long long)databus * (unsigned long long)temp->GetDT()) >> 32);
				break;

				// DIV
			case 0x05:
				// in case of zero division, set out zero
				if(temp->GetDT() == 0)
					out->Set(0);
				else
					out->Set(databus/temp->GetDT());
				break;

				// REM
			case 0x06:
				if(temp->GetDT() == 0)
					out->Set(0);
				else
					out->Set(databus % temp->GetDT());
				break;

				// Carry
			case 0x07:
				out->Set(databus+temp->GetDT() < Max(databus, temp->GetDT()));
				break;

				// Borrow
			case 0x08:
				out->Set(databus-temp->GetDT() < Min(databus, temp->GetDT()));
				break;

				// SADD (signed add)
			case 0x09:
				out->Set( (unsigned int) ( (signed)databus+(signed)temp->GetDT() ) );
				break;

				// SSUB
			case 0x0A:
				out->Set( (unsigned int) ( (signed)databus-(signed)temp->GetDT() ) );
				break;

				// SMULL
			case 0x0B:
				out->Set( (long long)databus*(long long)temp->GetDT() );
				break;

				// SMULL HIGH
			case 0x0C:
				out->Set( ( (unsigned long long)((long long)databus*(long long)temp->GetDT()) ) >> 32 );
				break;

				// SDIV
			case 0x0D:
				if(temp->GetDT() == 0)
					out->Set(0);
				else
					out->Set( (long long)databus/(long long)temp->GetDT());
				break;

				// SREM
			case 0x0E:
				if(temp->GetDT() == 0)
					out->Set(0);
				else
					out->Set( (long long)databus % (long long)temp->GetDT());
				break;

				// Signed carry
			case 0x0F:
				out->Set( ( (signed)databus+(signed)temp->GetDT() ) < Max((signed)databus, (signed)temp->GetDT()));
				break;

				// Signed borrow
			case 0x10:
				out->Set( ( (signed)databus-(signed)temp->GetDT() ) > Min((signed)databus, (signed)temp->GetDT()));
				break;

				// bitwise AND
			case 0x11:
				out->Set( databus&temp->GetDT() );
				break;

				// bitwise OR
			case 0x12:
				out->Set( databus|temp->GetDT() );
				break;

				// bitwise NOT
			case 0x13:
				out->Set(~databus);
				break;

				// bitwise XOR
			case 0x14:
				out->Set(databus^temp->GetDT());
				break;

				// rotate left (with carry)
			case 0x15:
				out->Set( (databus << (temp->GetDT()%33) ) | (databus >> (32-(temp->GetDT()%33) )) );
				break;
				
				// rotate right (with carry)
			case 0x16:
				out->Set( (databus >> (temp->GetDT()%33) ) | (databus << (32-(temp->GetDT()%33) )) );
				break;

				// logical and
			case 0x17:
				out->Set( databus && temp->GetDT() );
				break;

				// logical OR
			case 0x18:
				out->Set( databus || temp->GetDT() );
				break;

				// logical not
			case 0x19:
				out->Set(!databus);
				break;

				// logical XOR
			case 0x1A:
				out->Set( (databus != 0) ^ (temp->GetDT() != 0) );
				break;

				// shift left (no carry)
			case 0x1B:
				out->Set(databus << temp->GetDT());
				break;

				// shift right
			case 0x1C:
				out->Set(databus >> temp->GetDT());
				break;

				// bitwise NAND
			case 0x1D:
				out->Set( ~(databus & temp->GetDT()));
				break;

				// bitwise NOR
			case 0x1E:
				out->Set( ~(databus | temp->GetDT()));
				break;

				// bool
			case 0x1F:
				out->Set( databus != 0 );
				break;

				// max
			case 0x20:
				out->Set( Max(databus, temp->GetDT()));
				break;

				// which number is larger
			case 0x21:
				out->Set( temp->GetDT() > databus );
				break;

				// min
			case 0x22:
				out->Set( Min(databus, temp->GetDT()) );
				break;

				// which number is smaller
			case 0x23:
				out->Set( temp->GetDT() < databus );
				break;

				// signed max
			case 0x24:
				out->Set( Max((signed)databus, (signed)temp->GetDT()));
				break;

				// which signed number is larger
			case 0x25:
				out->Set( (signed)temp->GetDT() > (signed)databus );
				break;

				// signed min
			case 0x26:
				out->Set( Min((signed)databus, (signed)temp->GetDT()) );
				break;

				// which signed number is smaller
			case 0x27:
				out->Set( (signed)temp->GetDT() < (signed)databus );
				break;

				// EQL (equal)
			case 0x28:
				out->Set( databus == temp->GetDT() );
				break;

				// copy DATA to OUT when TEMP is zero
			case 0x29:
				if(!temp->GetDT())
					out->Set(databus);
				break;

				// copy DATA to OUT when TEMP is nonzero
			case 0x2A:
				if(temp->GetDT())
					out->Set(databus);
				break;
			}
		}
	}
}