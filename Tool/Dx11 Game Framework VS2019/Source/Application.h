#pragma once
#include "Base.h"

class RenderWindow;
class GraphicDevice;

class Application abstract : public Base
{
public:
	Application();
	virtual ~Application() override = default;

	LRESULT WindowProc(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND GetWindowHandle() const;

protected:
	virtual void destroy() override;

public:
	RenderWindow* mRenderWindow;
	GraphicDevice* mGraphicDevice;
};