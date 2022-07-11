#include <iostream>
#include "AttoWPU.h"
using namespace std;
using namespace attoWPU::simulator;
using namespace attoWPU::shared;

void main()
{
	AttoWPU WPU;
	for(;;)
	{
		WPU.Cycle();
		//WPU.Show();
		//cout << endl;
		cin.get();
	}
}