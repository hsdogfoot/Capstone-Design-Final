#pragma once
#include "Base.h"

#include "Camera.h"

class CameraManager final : public Base
{
public:
	static CameraManager* GetInstance();
	static uint32_t DestroyInstance();

	bool RegisterCamera(const WCHAR* tag, Camera* camera);
	void SetMainCamera(const WCHAR* tag);

	void Update();

private:
	CameraManager();
	virtual ~CameraManager() override = default;

	virtual void destroy() override;

public:
	Camera* MainCamera;

private:
	static CameraManager* mInstance;

	std::unordered_map<const WCHAR*, Camera*> mCameras;
};