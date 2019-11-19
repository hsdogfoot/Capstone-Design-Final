#pragma once
#include "Base.h"

#include "ComponentFactory.h"

class Component;

class GameObject : public Base
{
public:
	static GameObject* Create();

	template <typename T, typename... Args>
	bool EquipComponent(const WCHAR* protoKey, Args... arguments);

	virtual uint8_t Update();
	virtual void PreRelease();

protected:
	GameObject();
	virtual ~GameObject() override = default;

	virtual void destroy() override;

public:
	bool IsInvalid;

protected:
	std::vector<Component*> mComponents;
};

template<typename T, typename ...Args>
bool GameObject::EquipComponent(const WCHAR* protoKey, Args ...arguments)
{
	Component* clone = nullptr;
	if (!ComponentFactory::GetInstance()->GetClone<T>(protoKey, &clone, arguments...))
	{
		return false;
	}

	mComponents.push_back(clone);
	clone->SetOwner(this);

	return true;
}