//pre-computer header
#include "pch.h"

std::wstring StringtoWstring(std::string in)
{
	return std::wstring(in.begin(), in.end()).c_str();
}