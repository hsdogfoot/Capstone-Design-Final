#include "stdafx.h"
#include "InputManager.h"

InputManager* InputManager::mInstance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new InputManager();
	}

	return mInstance;
}

uint32_t InputManager::DestroyInstance()
{
	if (mInstance == nullptr)
	{
		return 0;
	}

	uint32_t refCount = mInstance->Release();
	if (refCount == 0)
	{
		mInstance = nullptr;
	}

	return refCount;
}

bool InputManager::Initialize(HINSTANCE hInstance, HWND windowHandle, HWND parentHandle, int screenWidth, int screenHeight)
{
	Assert(hInstance != nullptr);
	Assert(windowHandle != nullptr);
	Assert(parentHandle != nullptr);

	mParentHandle = parentHandle;
	mWindowHandle = windowHandle;

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	HRESULT hResult = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8W, reinterpret_cast<void**>(&mDirectInput), nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	if (!initializeKeyboard())
	{
		return false;
	}

	if (!initializeMouse())
	{
		return false;
	}

	return true;
}

void InputManager::ResetWindowSize(int screenWidth, int screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
}

XMFLOAT2 InputManager::GetMouseAtWorld() const
{
	float mouseX = mMousePos.x;
	float mouseY = mMousePos.y;

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, CameraManager::GetInstance()->MainCamera->GetProjection());

	XMFLOAT3 mouse;
	mouse.x = 2.0f * mouseX / mScreenWidth - 1.0f;
	mouse.y = 2.0f * mouseY / mScreenHeight - 1.0f;
	mouse.y *= -1.0f;
	mouse.z = 1.f;

	mouse.x /= projection._11;
	mouse.y /= projection._22;

	XMVECTOR point = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR dir = XMVectorSet(mouse.x, mouse.y, 1.f, 0.f) - point;
	dir = XMVector3Normalize(dir);

	XMMATRIX view = CameraManager::GetInstance()->MainCamera->GetView();
	view = XMMatrixInverse(nullptr, view);

	point = XMVector3TransformCoord(point, view);
	dir = XMVector3TransformNormal(dir, view);

	XMVECTOR meetPoint;
	float u = XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) - point)) / XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), dir));
	meetPoint = point + u * dir;

	XMFLOAT2 meetP;
	XMStoreFloat2(&meetP, meetPoint);

	return meetP;
}


void InputManager::Update()
{
	readKeyboard();
	readMouse();

	for (int i = 0; i < 256; ++i)
	{
		mPreviousKeyboardState[i] = mCurrentKeyboardState[i];

		if (mKeyboardState[i] & 0x80)
		{
			mCurrentKeyboardState[i] = true;
		}
		else
		{
			mCurrentKeyboardState[i] = false;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		mPreviousMouseState[i] = mCurrentMouseState[i];

		if (mMouseState.rgbButtons[i] & 0x80)
		{
			mCurrentMouseState[i] = true;
		}
		else
		{
			mCurrentMouseState[i] = false;
		}
	}
}

InputManager::InputManager()
	: Base()
	, mParentHandle(nullptr)
	, mWindowHandle(nullptr)
	, mDirectInput(nullptr)
	, mKeyboard(nullptr)
	, mKeyboardState()
	, mCurrentKeyboardState()
	, mPreviousKeyboardState()
	, mMouse(nullptr)
	, mMouseState()
	, mCurrentMouseState()
	, mPreviousMouseState()
	, mMousePos()
	, mScreenWidth(0)
	, mScreenHeight(0)
{
}

bool InputManager::initializeKeyboard()
{
	HRESULT hResult = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mKeyboard->SetCooperativeLevel(mParentHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mKeyboard->Acquire();
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

bool InputManager::initializeMouse()
{
	HRESULT hResult = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mMouse->SetCooperativeLevel(mParentHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mMouse->Acquire();
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

bool InputManager::readKeyboard()
{
	HRESULT hResult = mKeyboard->GetDeviceState(sizeof(mKeyboardState), reinterpret_cast<LPVOID>(&mKeyboardState));
	if (FAILED(hResult))
	{
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED)
		{
			mKeyboard->Acquire();
		}

		return false;
	}

	return true;
}

bool InputManager::readMouse()
{
	HRESULT hResult = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<LPVOID>(&mMouseState));
	if (FAILED(hResult))
	{
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED)
		{
			mMouse->Acquire();
		}

		return false;
	}

	GetCursorPos(&mMousePos);
	ScreenToClient(mWindowHandle, &mMousePos);

	//if (mMousePos.x < 0)
	//{
	//	mMousePos.x = 0;
	//}

	//if (mMousePos.y < 0)
	//{
	//	mMousePos.y = 0;
	//}

	//if (mMousePos.x > mScreenWidth)
	//{
	//	mMousePos.x = mScreenWidth;
	//}

	//if (mMousePos.y > mScreenHeight)
	//{
	//	mMousePos.y = mScreenHeight;
	//}

	return true;
}

void InputManager::destroy()
{
	if (mKeyboard != nullptr)
	{
		mKeyboard->Unacquire();
		mKeyboard->Release();
	}

	if (mMouse != nullptr)
	{
		mMouse->Unacquire();
		mMouse->Release();
	}

	ReleaseCOM(mDirectInput);
}
