#include "stdafx.h"
#include "GameObject.h"

#include <algorithm>
#include "Component.h"

GameObject* GameObject::Create()
{
	return new GameObject();
}

uint8_t GameObject::Update()
{
	if (IsInvalid)
	{
		return 1;
	}

	uint8_t exitCode = 0;
	for (auto component : mComponents)
	{
		exitCode |= component->Update();
	}

	return exitCode;
}

void GameObject::PreRelease()
{
	IsInvalid = true;

	for (auto component : mComponents)
	{
		component->Release();
	}

	mComponents.clear();
}

GameObject::GameObject()
	: Base()
	, IsInvalid(false)
{
}

void GameObject::destroy()
{
	for (auto component : mComponents)
	{
		component->Release();
	}
}

