#pragma once
#include "Base.h"

class Application;

class RenderWindow final : public Base
{
public:
	static bool Create(HINSTANCE hInstance, HWND parentHandle, std::string title, std::string className, int width, int height, Application* application, RenderWindow** outIstance);

private:
	RenderWindow() = delete;
	explicit RenderWindow(HINSTANCE hInstance, HWND parentHandle, std::string title, std::string className, int width, int height);
	virtual ~RenderWindow() override = default;

	bool initialize(Application* application);
	bool registerWindowClass();
	bool createWindow(Application* application);

	virtual void destroy() override;

public:
	HINSTANCE HInstance;
	HWND WindowHandle;
	HWND ParentHandle;

	int Width;
	int Height;

private:
	std::string mTitle;
	std::string mClassName;

	std::wstring mTitleWide;
	std::wstring mClassNameWide;
};