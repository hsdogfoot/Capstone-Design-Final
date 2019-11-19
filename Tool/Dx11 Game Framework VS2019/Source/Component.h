#pragma once
#include "Base.h"

class GameObject;

class Component abstract : public Base
{
public:
	Component();
	virtual ~Component() override = default;

	virtual void SetOwner(GameObject* owner);

	virtual uint8_t Update() = 0;
	virtual void ReadyRender() const;
	virtual void AfterRender() const;

	virtual bool CloneForPrefab(Component** outInstance) = 0;

protected:
	virtual void destroy() override;

public:
	GameObject* Owner;
};