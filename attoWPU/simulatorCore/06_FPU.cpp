#include "06_FPU.h"
#include "Values.h"
#include <cmath>
#include <cstdlib>

namespace attoWPU
{
	namespace simulator
	{
		Unit06_FPU::Unit06_FPU(Unit05_OUT *outRegister, Unit02_TEMP *tempRegister) : Unit(0x06)
		{
			databus = 0;
			out = outRegister;
			temp = tempRegister;
		}

		void Unit06_FPU::Output(float val)
		{
			out->Set(*reinterpret_cast<unsigned int *>(&val));
		}

		void Unit06_FPU::Execute(byte code)
		{
			unsigned int t = temp->GetDT();
			float data = *reinterpret_cast<float *>(&databus),
				tmp = *reinterpret_cast<float*>( &t ),
				zero = 0.0F;

			switch(code & 0x1F)
			{
				// Zero (Lelouch Vi Britannia commands you, 0x00!!! :3 )
			case 0x00:
				Output(0.0F);
				break;

				// FADD
			case 0x01:
				Output(data+tmp);
				break;

				// FSUB
			case 0x02:
				Output(data-tmp);
				break;

				// FMUL
			case 0x03:
				Output(data*tmp);
				break;

				// FDIV
			case 0x04:
				Output(data/tmp);
				break;

				// FSIN
			case 0x05:
				Output(std::sin(data));
				break;

				// FTAN
			case 0x06:
				Output(std::tan(data));
				break;

				// FEXP
			case 0x07:
				Output(std::exp(data));
				break;

				// FSQRT
			case 0x08:
				Output(std::sqrt(data));
				break;

				// FLOG2
			case 0x09:
				Output(std::log10(data)/std::log10(2.0F));
				break;

				// FLOG10
			case 0x0A:
				Output(std::log10(data));
				break;

				// FLN (natural logarithm)
			case 0x0B:
				Output(std::log(data));
				break;

				// is infinity
			case 0x0C:
				out->Set( !(data == (1.0F/zero) || data == (-1.0F/zero) ) );
				break;

				// Convert to int
			case 0x0D:
				out->Set((int)data);
				break;

				// Convert int to float
			case 0x0E:
				Output( (float)((int)databus) );
				break;

				// Max
			case 0x0F:
				Output( Max(data, tmp) );
				break;

				// which is larger?
			case 0x10:
				out->Set( tmp > data );
				break;

				// Min
			case 0x11:
				Output( Min(data, tmp) );
				break;

				// which is smaller?
			case 0x12:
				out->Set( tmp < data );
				break;

				// abs
			case 0x13:
				Output(std::abs(data));
				break;

				// power
			case 0x14:
				Output(std::pow(data, tmp));
				break;
			}
		}
	}
}