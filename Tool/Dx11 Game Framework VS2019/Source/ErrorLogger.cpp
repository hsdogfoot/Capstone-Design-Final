#include "stdafx.h"
#include "ErrorLogger.h"

#include <comdef.h>
#include "StringConverter.h"

void ErrorLogger::Log(std::string file, std::string function, int line, std::string message)
{
	std::string errorMsg = "Error\t: " + message + "\n\nFile\t: " + file + "\nFunction\t: " + function + "\nLine\t: " + std::to_string(line);
	MessageBoxExA(nullptr, errorMsg.c_str(), "Error occurred.", MB_ICONERROR, 0);
}

void ErrorLogger::LogHResult(std::string file, std::string function, int line, HRESULT hResult)
{
	_com_error error(hResult);
	std::wstring errorMsg = L"Error\t: " + std::wstring(error.ErrorMessage()) + L"\n\nFile\t: " + StringConverter::ConvertStringToWide(file) + L"\nFunction\t: " + StringConverter::ConvertStringToWide(function) + L"\nLine\t: " + std::to_wstring(line);
	MessageBoxExW(nullptr, errorMsg.c_str(), L"Error occurred.", MB_ICONERROR, 0);
}

void ErrorLogger::LogHResultAndPath(std::string file, std::string function, int line, HRESULT hResult, std::wstring path)
{
	_com_error error(hResult);
	std::wstring errorMsg = L"Error\t: " + std::wstring(error.ErrorMessage()) + L"\nPath\t: " + path + L"\n\nFile\t: " + StringConverter::ConvertStringToWide(file) + L"\nFunction\t: " + StringConverter::ConvertStringToWide(function) + L"\nLine\t: " + std::to_wstring(line);
	MessageBoxExW(nullptr, errorMsg.c_str(), L"Error occurred.", MB_ICONERROR, 0);
}
