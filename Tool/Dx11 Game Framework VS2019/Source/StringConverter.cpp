#include "stdafx.h"
#include "StringConverter.h"

std::wstring StringConverter::ConvertStringToWide(std::string str)
{
	return std::wstring(str.begin(), str.end());
}