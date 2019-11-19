#include "stdafx.h"
#include "CamComponent.h"

#include "Camera.h"

const XMVECTOR CamComponent::DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR CamComponent::DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR CamComponent::DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

CamComponent* CamComponent::Create()
{
	return new CamComponent();
}

bool CamComponent::Clone(Component** outInstance, XMFLOAT2 lookAt, float viewWidth, float viewHeight, float nearZ, float farZ, float zoomSpeed)
{
	CamComponent* instance = new CamComponent(lookAt, viewWidth, viewHeight, nearZ, farZ, zoomSpeed);
	if (!instance->initialize())
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void CamComponent::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);
	static_cast<Camera*>(Owner)->SetCamComponent(this);
}

uint8_t CamComponent::Update()
{
	return 0;
}

void CamComponent::SetLookAtPos(XMFLOAT2 lookAt)
{
	LookAt = XMLoadFloat2(&lookAt);
	Eye = LookAt - DEFAULT_FORWARD_VECTOR;

	updateView();
}

void CamComponent::AdjustLookAtPos(XMFLOAT2 posDelta)
{
	LookAt += XMLoadFloat2(&posDelta);
	Eye = LookAt - DEFAULT_FORWARD_VECTOR;

	updateView();
}

void CamComponent::SetProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	mViewWidth = viewWidth;
	mViewHeight = viewHeight;
	mNearZ = nearZ;
	mFarZ = farZ;

	Projection = XMMatrixOrthographicLH(mViewWidth, mViewHeight, mNearZ, mFarZ);
}

void CamComponent::ZoomIn(float& timeDelta)
{
	mViewWidth -= timeDelta * ZoomSpeed;
	mViewHeight -= timeDelta * ZoomSpeed * mAspectRatio;

	if (mViewWidth < 0.5f)
	{
		mViewWidth = 0.5f;
		mViewHeight = 0.5f * mAspectRatio;
	}

	updateProjection();
}

void CamComponent::ZoomOut(float& timeDelta)
{
	mViewWidth += timeDelta * ZoomSpeed;
	mViewHeight += timeDelta * ZoomSpeed * mAspectRatio;

	updateProjection();
}

bool CamComponent::CloneForPrefab(Component** outInstance)
{
	CamComponent* clone = new CamComponent(*this);

	*outInstance = clone;

	return true;
}

CamComponent::CamComponent()
	: Component()
	, Eye(XMVectorZero())
	, LookAt(XMVectorZero())
	, View(XMMatrixIdentity())
	, Projection(XMMatrixIdentity())
	, ZoomSpeed(0.0f)
	, mViewWidth(0.0f)
	, mViewHeight(0.0f)
	, mNearZ(0.0f)
	, mFarZ(0.0f)
	, mAspectRatio(0.0f)
{
}

CamComponent::CamComponent(XMFLOAT2 lookAt, float viewWidth, float viewHeight, float nearZ, float farZ, float zoomSpeed)
	: Component()
	, Eye(XMVectorZero())
	, LookAt(XMLoadFloat2(&lookAt))
	, View(XMMatrixIdentity())
	, Projection(XMMatrixIdentity())
	, ZoomSpeed(zoomSpeed)
	, mViewWidth(viewWidth)
	, mViewHeight(viewHeight)
	, mNearZ(nearZ)
	, mFarZ(farZ)
	, mAspectRatio(viewHeight / viewWidth)
{
}

CamComponent::CamComponent(const CamComponent& other)
	: Component()
	, Eye(other.Eye)
	, LookAt(other.LookAt)
	, View(other.View)
	, Projection(other.Projection)
	, ZoomSpeed(other.ZoomSpeed)
	, mViewWidth(other.mViewWidth)
	, mViewHeight(other.mViewHeight)
	, mNearZ(other.mNearZ)
	, mFarZ(other.mFarZ)
	, mAspectRatio(other.mAspectRatio)
{
}

void* CamComponent::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void CamComponent::operator delete(void* memory)
{
	_aligned_free(memory);
}

bool CamComponent::initialize()
{
	updateProjection();
	
	XMFLOAT2 lookAt;
	XMStoreFloat2(&lookAt, LookAt);
	SetLookAtPos(lookAt);

	return true;
}

void CamComponent::destroy()
{
	Component::destroy();
}
