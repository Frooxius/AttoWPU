#pragma once
#include <string>

template <class X> X Min(X a, X b)
{
	return a>b?b:a;
}

template <class X> X Max(X a, X b)
{
	return a>b?a:b;
}

template <class X> X Limit(X val, X min, X max)
{
	return (val>min?val:min)<max?val:max;
}

std::string IntToStr(int num);

bool True();