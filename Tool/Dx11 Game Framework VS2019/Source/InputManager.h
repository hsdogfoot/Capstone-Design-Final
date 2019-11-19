#pragma once
#include "Base.h"

class InputManager final : public Base
{
public:
	enum class eMouseKeyType { Left, Right, Wheel };
	enum { KEYBOARD_SIZE = 256, MOUSE_SIZE = 3 };

public:
	static InputManager* GetInstance();
	static uint32_t DestroyInstance();

	bool Initialize(HINSTANCE hInstance, HWND windowHandle, HWND parentHandle, int screenWidth, int screenHeight);

	void ResetWindowSize(int screenWidth, int screenHeight);

	inline bool IsKeyDown(unsigned char keyCode) const;
	inline bool IsKeyPressed(unsigned char keyCode) const;
	inline bool IsKeyUp(unsigned char keyCode) const;

	inline bool IsMouseDown(eMouseKeyType key) const;
	inline bool IsMousePressed(eMouseKeyType key) const;
	inline bool IsMouseUp(eMouseKeyType key) const;

	inline int GetMouseX() const;
	inline int GetMouseY() const;

	XMFLOAT2 GetMouseAtWorld() const;

	void Update();

private:
	InputManager();
	virtual ~InputManager() override = default;

	bool initializeKeyboard();
	bool initializeMouse();

	bool readKeyboard();
	bool readMouse();

	virtual void destroy() override;

private:
	static InputManager* mInstance;

	HWND mParentHandle;
	HWND mWindowHandle;

	IDirectInput8W* mDirectInput;

	IDirectInputDevice8W* mKeyboard;
	unsigned char mKeyboardState[KEYBOARD_SIZE];
	bool mCurrentKeyboardState[KEYBOARD_SIZE];
	bool mPreviousKeyboardState[KEYBOARD_SIZE];

	IDirectInputDevice8W* mMouse;
	DIMOUSESTATE mMouseState;
	bool mCurrentMouseState[MOUSE_SIZE];
	bool mPreviousMouseState[MOUSE_SIZE];
	POINT mMousePos;

public:
	int mScreenWidth;
	int mScreenHeight;
};

inline bool InputManager::IsKeyDown(unsigned char keyCode) const
{
	return !mPreviousKeyboardState[keyCode] && mCurrentKeyboardState[keyCode];
}

inline bool InputManager::IsKeyPressed(unsigned char keyCode) const
{
	return mPreviousKeyboardState[keyCode] && mCurrentKeyboardState[keyCode];
}

inline bool InputManager::IsKeyUp(unsigned char keyCode) const
{
	return mPreviousKeyboardState[keyCode] && !mCurrentKeyboardState[keyCode];
}

inline bool InputManager::IsMouseDown(eMouseKeyType key) const
{
	return !mPreviousMouseState[static_cast<int>(key)] && mCurrentMouseState[static_cast<int>(key)];
}

inline bool InputManager::IsMousePressed(eMouseKeyType key) const
{
	return mPreviousMouseState[static_cast<int>(key)] && mCurrentMouseState[static_cast<int>(key)];
}

inline bool InputManager::IsMouseUp(eMouseKeyType key) const
{
	return mPreviousMouseState[static_cast<int>(key)] && !mCurrentMouseState[static_cast<int>(key)];
}

inline int InputManager::GetMouseX() const
{
	return mMousePos.x;
}

inline int InputManager::GetMouseY() const
{
	return mMousePos.y;
}