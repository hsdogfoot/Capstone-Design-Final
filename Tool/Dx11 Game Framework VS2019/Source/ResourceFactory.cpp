#include "stdafx.h"
#include "ResourceFactory.h"

ResourceFactory* ResourceFactory::mInstance = nullptr;

ResourceFactory* ResourceFactory::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ResourceFactory();
	}

	return mInstance;
}

uint32_t ResourceFactory::DestroyInstance()
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

bool ResourceFactory::Initialize()
{
	if (!registerPrototypes())
	{
		return false;
	}

	return true;
}

bool ResourceFactory::RegisterPrototype(const WCHAR* tag, Resource* prototype)
{
	Assert(tag != nullptr);
	Assert(prototype != nullptr);

	if (mPrototypes.find(tag) != mPrototypes.end())
	{
		return false;
	}

	mPrototypes.insert(std::pair<const WCHAR*, Resource*>(tag, prototype));

	return true;
}

bool ResourceFactory::RemovePrototype(const WCHAR* tag)
{
	auto iter = mPrototypes.find(tag);
	if (iter == mPrototypes.end())
	{
		return false;
	}

	iter->second->Release();
	mPrototypes.erase(iter);

	return true;
}

bool ResourceFactory::GetClone(const WCHAR* tag, Resource** outInstance) const
{
	auto iter = mPrototypes.begin();
	for (; iter != mPrototypes.end(); ++iter)
	{
		if (!wcscmp(iter->first, tag))
		{
			break;
		}
	}

	if (iter == mPrototypes.end())
	{
		ErrorLog("Resource prototype find failed.");

		return false;
	}

	//auto iter = mPrototypes.find(tag);
	//if (iter == mPrototypes.end())
	//{
	//	ErrorLog("Resource prototype find failed.");

	//	return false;
	//}

	*outInstance = iter->second->Clone();

	return true;
}

ResourceFactory::ResourceFactory()
	: Base()
{
}

bool ResourceFactory::registerPrototypes()
{
	bool bResult = true;

	return bResult;
}

void ResourceFactory::destroy()
{
	for (auto pair : mPrototypes)
	{
		pair.second->Release();
	}
}
