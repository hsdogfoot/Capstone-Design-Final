#include "stdafx.h"
#include "Camera.h"

Camera* Camera::Create()
{
	return new Camera();
}

void Camera::SetCamComponent(CamComponent* camComponent)
{
	Assert(camComponent != nullptr);

	if (mCamComponent != nullptr)
	{
		ErrorLog("CamComponent access error.");

		return;
	}

	mCamComponent = camComponent;
	mCamComponent->AddRefCount();

	IsInvalid = false;
}

void Camera::PreRelease()
{
	GameObject::PreRelease();

	ReleaseBase(mCamComponent);
	mCamComponent = nullptr;
}

Camera::Camera()
	: GameObject()
	, mCamComponent(nullptr)
{
	IsInvalid = true;
}

void Camera::destroy()
{
	ReleaseBase(mCamComponent);

	GameObject::destroy();
}
