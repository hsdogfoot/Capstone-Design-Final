#pragma once
#include "Base.h"

class Actor;

class PickingManager final : public Base
{
public:
	static PickingManager* GetInstance();
	static uint32_t DestroyInstance();

	void Initialize(int screenWidth, int screenHeight);

	void ResetWindowSize(int screenWidth, int screenHeight);

	void Pick();

private:
	PickingManager();
	virtual ~PickingManager() override = default;

	bool isInRect(XMVECTOR planeNormal, XMVECTOR planePoint, XMVECTOR* vertices);

	virtual void destroy() override;

public:
	Actor* SelectedActor;

private:
	static PickingManager* mInstance;

	float mScreenWidth;
	float mScreenHeight;
};