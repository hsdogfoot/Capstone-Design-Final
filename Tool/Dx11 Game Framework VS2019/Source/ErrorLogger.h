#pragma once

#define __FILENAME__ (strrchr(__FILE__,'\\') + 1)
#define __FILENAME_W__ (wcsrchr(_CRT_WIDE(__FILE__),'\\') + 1) 

#define ErrorLog(message) ErrorLogger::Log(__FILENAME__, __FUNCTION__, __LINE__, message)
#define HRLog(hResult) ErrorLogger::LogHResult(__FILENAME__, __FUNCTION__, __LINE__, hResult);
#define HRAndPathLog(hResult, path) ErrorLogger::LogHResultAndPath(__FILENAME__, __FUNCTION__, __LINE__, hResult, path);

#if defined(DEBUG) || defined(_DEBUG)
#define Assert(exp)	if (!(exp))																										\
					{																												\
						wchar_t message[256];																						\
						wsprintf(message, L"\nFile\t\t: %s\nFunction\t\t: %s()\nLine\t\t: %d\nExpression\t: %s\n"					\
									, __FILENAME_W__, _CRT_WIDE(__FUNCTION__), __LINE__, _CRT_WIDE(#exp));							\
						MessageBoxEx(nullptr, message, L"Assertion Failed", MB_ICONERROR, 0);										\
						__asm { int 3 }																								\
					}
#else
#define Assert(exp)	( __assume(exp) )
#endif

class ErrorLogger final abstract
{
public:
	static void Log(std::string file, std::string function, int line, std::string message);
	static void LogHResult(std::string file, std::string function, int line, HRESULT hResult);
	static void LogHResultAndPath(std::string file, std::string function, int line, HRESULT hResult, std::wstring path);
};