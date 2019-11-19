#include "stdafx.h"
#include "PrefabManager.h"

PrefabManager* PrefabManager::mInstance = nullptr;

PrefabManager* PrefabManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new PrefabManager();
	}

	return mInstance;
}

uint32_t PrefabManager::DestroyInstance()
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

bool PrefabManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	mDevice = device;
	mDevice->AddRef();
	mDeviceContext = deviceContext;
	mDeviceContext->AddRef();

	return true;
}

bool PrefabManager::CheckPrefab(const WCHAR* prefabTag)
{
	for (auto pair : mPrefabs)
	{
		if (!wcscmp(prefabTag, pair.first))
		{
			return true;
		}
	}

	return false;
}

bool PrefabManager::RegisterPrefab(const WCHAR* prefabTag, XMFLOAT2 scale, float rotation, const WCHAR* textureTag)
{
	Assert(prefabTag != nullptr);
	Assert(textureTag != nullptr);

	for (auto pair : mPrefabs)
	{
		if (!wcscmp(prefabTag, pair.first))
		{
			MessageBoxA(nullptr, "Prefab tag is already used.", "Alert", MB_OK);
			return false;
		}
	}

	Actor* actor = Actor::Create(prefabTag);
	if (!actor->EquipComponent<Transform>(L"Transform", scale, XMFLOAT2(rotation, 0.0f), XMFLOAT2(0.0f, 0.0f)))
	{
		actor->PreRelease();
		actor->Release();

		return false;
	}

	if (!actor->EquipComponent<Renderer>(L"Renderer", mDevice, mDeviceContext, L"VS_Default", L"PS_Default", textureTag))
	{
		actor->PreRelease();
		actor->Release();

		return false;
	}

	if (!actor->EquipComponent<ColorEffect>(L"ColorEffect", mDevice, mDeviceContext))
	{
		actor->PreRelease();
		actor->Release();

		return false;
	}

	mPrefabs.insert(std::pair<const WCHAR*, Actor*>(prefabTag, actor));
	mPrefabVec.push_back(actor);

	return false;
}

bool PrefabManager::SetupPrefab(const WCHAR* prefabTag)
{
	Assert(prefabTag != nullptr);

	auto iter = mPrefabs.begin();
	for (; iter != mPrefabs.end(); ++iter)
	{
		if (!wcscmp(iter->first, prefabTag))
		{
			break;
		}
	}

	if (iter == mPrefabs.end())
	{
		return false;
	}

	SelectedPrefab = iter->second;
	SelectedPrefab->GetColorEffect()->SetAlpha(0.5f);
	SelectedPrefab->GetColorEffect()->SetAlphaBlendingMode(true);
	SelectedPrefab->AddRefCount();

	return true;
}

bool PrefabManager::SetupPrefab(uint32_t index)
{
	SelectedPrefab = mPrefabVec[index];
	SelectedPrefab->GetColorEffect()->SetAlpha(0.5f);
	SelectedPrefab->GetColorEffect()->SetAlphaBlendingMode(true);
	SelectedPrefab->AddRefCount();

	return true;
}

void PrefabManager::RenderSelectedPrefab(XMFLOAT2 position)
{
	if (SelectedPrefab == nullptr)
	{
		return;
	}

	SelectedPrefab->GetTransform()->Position = XMLoadFloat2(&position);
	SelectedPrefab->Update();
	SelectedPrefab->ReadyRender();
	SelectedPrefab->Render();
	SelectedPrefab->AfterRender();
}

PrefabManager::PrefabManager()
	: Base()
	, SelectedPrefab(nullptr)
	, mDevice(nullptr)
	, mDeviceContext(nullptr)
{
}

void PrefabManager::destroy()
{
	ReleaseBase(SelectedPrefab);

	for (auto pair : mPrefabs)
	{
		pair.second->PreRelease();
		pair.second->Release();
	}

	ReleaseCOM(mDevice);
	ReleaseCOM(mDeviceContext);
}
