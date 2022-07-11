#include "Values.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

std::string IntToStr(int num)
{
	char numbuf[12];
	std::string numstr = std::string();
	sprintf(numbuf, "%d", num);
	return numstr = numbuf;
}