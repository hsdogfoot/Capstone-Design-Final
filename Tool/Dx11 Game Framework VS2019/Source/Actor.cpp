#include "stdafx.h"
#include "Actor.h"

Actor* Actor::Create(const WCHAR* prefabName)
{
	return new Actor(prefabName);
}

bool Actor::Clone(Actor** outInstance)
{
	Actor* instance = new Actor(PrefabName);

	Component* clone = nullptr;
	for (auto component : mComponents)
	{
		if (!component->CloneForPrefab(&clone))
		{
			instance->PreRelease();
			instance->Release();

			return false;
		}

		instance->mComponents.push_back(clone);
		clone->SetOwner(instance);
	}

	*outInstance = instance;

	return true;
}

void Actor::SetRenderer(Renderer* renderer)
{
	Assert(renderer != nullptr);

	if (mRenderer != nullptr)
	{
		ErrorLog("Renderer access error.");

		return;
	}

	mRenderer = renderer;
	mRenderer->AddRefCount();

	IsInvalid = false;
}

void Actor::SetTransform(Transform* transform)
{
	Assert(transform != nullptr);

	if (mTransform != nullptr)
	{
		ErrorLog("Transform access error.");

		return;
	}

	mTransform = transform;
	mTransform->AddRefCount();

	IsInvalid = false;
}

void Actor::SetColorEffect(ColorEffect* colorEffect)
{
	Assert(colorEffect != nullptr);

	if (mColorEffect != nullptr)
	{
		ErrorLog("ColorEffect access error.");

		return;
	}

	mColorEffect = colorEffect;
	mColorEffect->AddRefCount();

	IsInvalid = false;
}

void Actor::SetPattern(Pattern* pattern)
{
	Assert(pattern != nullptr);

	if (mPattern != nullptr)
	{
		ErrorLog("Pattern access error.");

		return;
	}

	mPattern = pattern;
	mPattern->AddRefCount();
}

void Actor::ReadyRender() const
{
	if (IsInvalid)
	{
		return;
	}

	for (auto component : mComponents)
	{
		component->ReadyRender();
	}
}

void Actor::Render() const
{
	if (IsInvalid)
	{
		return;
	}

	mRenderer->Render();
}

void Actor::AfterRender() const
{
	if (IsInvalid)
	{
		return;
	}

	for (auto component : mComponents)
	{
		component->AfterRender();
	}
}

void Actor::PreRelease()
{
	GameObject::PreRelease();

	ReleaseBase(mRenderer);
	ReleaseBase(mTransform);
	ReleaseBase(mPattern);

	mRenderer = nullptr;
	mTransform = nullptr;
	mPattern = nullptr;
}

Actor::Actor()
	: GameObject()
	, mRenderer(nullptr)
	, mTransform(nullptr)
	, mColorEffect(nullptr)
	, mPattern(nullptr)
{
	IsInvalid = true;
}

Actor::Actor(const WCHAR* prefabName)
	: GameObject()
	, mRenderer(nullptr)
	, mTransform(nullptr)
	, mColorEffect(nullptr)
	, mPattern(nullptr)
{
	PrefabName = new WCHAR[wcslen(prefabName) + 1];
	wcscpy(PrefabName, prefabName);
}

void Actor::destroy()
{
	ReleaseBase(mRenderer);
	ReleaseBase(mTransform);
	ReleaseBase(mColorEffect);
	ReleaseBase(mPattern);

	GameObject::destroy();
}
