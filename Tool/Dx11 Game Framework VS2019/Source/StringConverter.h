#pragma once

class StringConverter final abstract
{
public:
	static std::wstring ConvertStringToWide(std::string str);
};