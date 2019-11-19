#include "stdafx.h"
#include "ActorManager.h"

#include "RenderManager.h"

ActorManager* ActorManager::mInstance = nullptr;

ActorManager* ActorManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ActorManager();
	}

	return mInstance;
}

uint32_t ActorManager::DestroyInstance()
{
	if (mInstance == nullptr)
	{
		return 0;
	}

	uint32_t refCount = mInstance->Release();
	if (refCount == 0)
	{
		mInstance = nullptr;
	}

	return refCount;
}

void ActorManager::RegisterActor(Actor* actor)
{
	Assert(actor != nullptr);

	mActors.push_back(actor);
}

void ActorManager::Update()
{
	uint8_t errorCode = 0;
	for (auto iter = mActors.begin(); iter != mActors.end(); )
	{
		errorCode = (*iter)->Update();
		if (errorCode != 0)
		{
			(*iter)->PreRelease();
			(*iter)->Release();

			iter = mActors.erase(iter);

			continue;
		}

		++iter;
	}

	readyRendering();
}

int ActorManager::GetActorCount() const
{
	return mActors.size();
}

Actor* ActorManager::GetActor(int index) const
{
	return mActors[index];
}

ActorManager::ActorManager()
	: Base()
{
}

void ActorManager::readyRendering()
{
	RenderManager* renderer = RenderManager::GetInstance();
	for (auto actor : mActors)
	{
		renderer->RegisterActor(actor);
	}
}

void ActorManager::destroy()
{
	for (auto actor : mActors)
	{
		actor->PreRelease();
		actor->Release();
	}
}
