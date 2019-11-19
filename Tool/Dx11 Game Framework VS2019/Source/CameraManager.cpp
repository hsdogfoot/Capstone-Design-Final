#include "stdafx.h"
#include "CameraManager.h"

CameraManager* CameraManager::mInstance = nullptr;

CameraManager* CameraManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new CameraManager();
	}

	return mInstance;
}

uint32_t CameraManager::DestroyInstance()
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

bool CameraManager::RegisterCamera(const WCHAR* tag, Camera* camera)
{
	Assert(tag != nullptr);
	Assert(camera != nullptr);

	if (mCameras.find(tag) != mCameras.end())
	{
		return false;
	}

	mCameras.insert(std::pair<const WCHAR*, Camera*>(tag, camera));

	return true;
}

void CameraManager::SetMainCamera(const WCHAR* tag)
{
	Assert(tag != nullptr);

	auto iter = mCameras.find(tag);
	if (iter == mCameras.end())
	{
		ErrorLog("Camera find failed.");

		return;
	}

	ReleaseBase(MainCamera);

	MainCamera = iter->second;
	MainCamera->AddRefCount();
}

void CameraManager::Update()
{
	uint8_t errorCode = 0;
	for (auto iter = mCameras.begin(); iter != mCameras.end(); )
	{
		errorCode = iter->second->Update();
		if (errorCode != 0)
		{
			iter->second->PreRelease();
			iter->second->Release();

			iter = mCameras.erase(iter);

			continue;
		}

		++iter;
	}
}

CameraManager::CameraManager()
	: Base()
	, MainCamera(nullptr)
{
}

void CameraManager::destroy()
{
	ReleaseBase(MainCamera);

	for (auto pair : mCameras)
	{
		pair.second->PreRelease();
		pair.second->Release();
	}
}
