#include "stdafx.h"
#include "Component.h"

#include "GameObject.h"

Component::Component()
	: Base()
	, Owner(nullptr)
{
}

void Component::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	if (Owner != nullptr)
	{
		return;
	}

	Owner = owner;
	Owner->AddRefCount();
}

void Component::ReadyRender() const
{
}

void Component::AfterRender() const
{
}

void Component::destroy()
{
	ReleaseBase(Owner);
}