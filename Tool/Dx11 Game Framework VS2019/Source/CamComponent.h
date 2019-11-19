#pragma once
#include "Component.h"

class CamComponent final : public Component
{
public:
	static CamComponent* Create();
	bool Clone(Component** outInstance, XMFLOAT2 lookAt, float viewWidth, float viewHeight, float nearZ, float farZ, float zoomSpeed);

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;

	void SetLookAtPos(XMFLOAT2 lookAt);
	void AdjustLookAtPos(XMFLOAT2 posDelta);
	void SetProjection(float viewWidth, float viewHeight, float nearZ, float farZ);
	void ZoomIn(float& timeDelta);
	void ZoomOut(float& timeDelta);

	virtual bool CloneForPrefab(Component** outInstance) override;

private:
	CamComponent();
	explicit CamComponent(XMFLOAT2 lookAt, float viewWidth, float viewHeight, float nearZ, float farZ, float zoomSpeed);
	explicit CamComponent(const CamComponent& other);
	virtual ~CamComponent() override = default;

	void* operator new(size_t size);
	void operator delete(void* memory);

	bool initialize();

	inline void updateView();
	inline void updateProjection();

	virtual void destroy() override;

public:
	XMVECTOR Eye;
	XMVECTOR LookAt;

	XMMATRIX View;
	XMMATRIX Projection;

	float ZoomSpeed;

	static const XMVECTOR DEFAULT_RIGHT_VECTOR;
	static const XMVECTOR DEFAULT_UP_VECTOR;
	static const XMVECTOR DEFAULT_FORWARD_VECTOR;

private:
	float mViewWidth;
	float mViewHeight;
	float mNearZ;
	float mFarZ;

	float mAspectRatio;
};

inline void CamComponent::updateView()
{
	View = XMMatrixLookAtLH(Eye, LookAt, DEFAULT_UP_VECTOR);
}

inline void CamComponent::updateProjection()
{
	Projection = XMMatrixOrthographicLH(mViewWidth, mViewHeight, mNearZ, mFarZ);
}
