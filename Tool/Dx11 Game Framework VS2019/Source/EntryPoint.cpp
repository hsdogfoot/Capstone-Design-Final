#include "stdafx.h"
#include "Game.h"

#include "RenderWindow.h"

static Game* g_instance = nullptr;

extern "C" __declspec(dllexport) void PrintMessage(LPCTSTR message)
{
	MessageBox(nullptr, message, L"Message from Game Framework", MB_OK);
}

// 게임을 생성하고 초기화합니다.
extern "C" __declspec(dllexport) bool CreateGame(HINSTANCE hInstance, HWND parentHandle, int windowWidth, int windowHeight)
{
	if (!Game::Create(hInstance, parentHandle, "Game Preview", "Game Preview", windowWidth, windowHeight, &g_instance))
	{
		MessageBox(nullptr, L"Game instance create failed. Application will be shut down...", L"Message from Game Framework", MB_ICONERROR);
		return false;
	}
	else if (g_instance == nullptr)
	{
		MessageBox(nullptr, L"Game instance create failed. Application will be shut down...", L"Message from Game Framework", MB_ICONERROR);
		return false;
	}

	//MessageBox(nullptr, L"Game instance create successed.", L"Message from Game Framework", MB_OK);

	return true;
}

// 게임의 메인 루프입니다.
extern "C" __declspec(dllexport) void UpdateGame()
{
	Assert(g_instance != nullptr);

	g_instance->Update();
}

// 게임 화면을 출력합니다.
extern "C" __declspec(dllexport) void RenderGame()
{
	Assert(g_instance != nullptr);

	g_instance->Render();
}

extern "C" __declspec(dllexport) void DestroyGame()
{
	ReleaseBase(g_instance);
	g_instance = nullptr;

	MessageBox(nullptr, L"Game instance destroy successed.", L"Message from Game Framework", MB_OK);
}

extern "C" __declspec(dllexport) HWND GetWindowHandle()
{
	Assert(g_instance != nullptr);

	return g_instance->GetWindowHandle();
}

extern "C" __declspec(dllexport) void SaveData()
{
	Assert(g_instance != nullptr);

	g_instance->SaveData();

	MessageBox(nullptr, L"Game data save successed.", L"Message from Game Framework", MB_OK);
}

extern "C" __declspec(dllexport) void TestingCode()
{
	Assert(g_instance != nullptr);

	g_instance->TestingCode();
}

extern "C" __declspec(dllexport) void RegisterPrefab(LPCTSTR path, LPCTSTR name)
{
	Assert(g_instance != nullptr);

	g_instance->RegisterPrefab(path, name);
}