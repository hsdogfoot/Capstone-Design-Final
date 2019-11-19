#pragma once
#include "GameObject.h"

class CamComponent;

class Camera : public GameObject
{
public:
	static Camera* Create();

	void SetCamComponent(CamComponent* camComponent);

	inline XMMATRIX GetView() const;
	inline XMMATRIX GetProjection() const;

	inline void SetLookAtPos(XMFLOAT2 lookAt);
	inline void AdjustLookAtPos(XMFLOAT2 posDelta);
	inline void SetProjection(float viewWidth, float viewHeight, float nearZ, float farZ);
	inline void ZoomIn(float& timeDelta);
	inline void ZoomOut(float& timeDelta);

	virtual void PreRelease() override;

protected:
	Camera();
	virtual ~Camera() override = default;

	virtual void destroy() override;

protected:
	CamComponent* mCamComponent;
};

inline XMMATRIX Camera::GetView() const
{
	return mCamComponent->View;
}

inline XMMATRIX Camera::GetProjection() const
{
	return mCamComponent->Projection;
}

inline void Camera::SetLookAtPos(XMFLOAT2 lookAt)
{
	mCamComponent->SetLookAtPos(lookAt);
}

inline void Camera::AdjustLookAtPos(XMFLOAT2 posDelta)
{
	mCamComponent->AdjustLookAtPos(posDelta);
}

inline void Camera::SetProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	mCamComponent->SetProjection(viewWidth, viewHeight, nearZ, farZ);
}

inline void Camera::ZoomIn(float& timeDelta)
{
	mCamComponent->ZoomIn(timeDelta);
}

inline void Camera::ZoomOut(float& timeDelta)
{
	mCamComponent->ZoomOut(timeDelta);
}
