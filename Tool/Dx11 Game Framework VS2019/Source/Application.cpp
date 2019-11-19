#include "stdafx.h"
#include "Application.h"

#include "GraphicDevice.h"
#include "RenderWindow.h"

Application::Application()
	: Base()
	, mRenderWindow(nullptr)
	, mGraphicDevice(nullptr)
{
}

LRESULT Application::WindowProc(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWND, uMsg, wParam, lParam);
}

HWND Application::GetWindowHandle() const
{
	return mRenderWindow->WindowHandle;
}

void Application::destroy()
{
	ReleaseBase(mRenderWindow);
	ReleaseBase(mGraphicDevice);
}
