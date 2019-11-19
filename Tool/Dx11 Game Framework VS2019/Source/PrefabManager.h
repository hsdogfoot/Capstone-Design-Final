#pragma once
#include "Base.h"

class Actor;

class PrefabManager final : public Base
{
public:
	static PrefabManager* GetInstance();
	static uint32_t DestroyInstance();
	
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	bool CheckPrefab(const WCHAR* prefabTag);
	bool RegisterPrefab(const WCHAR* prefabTag, XMFLOAT2 scale, float rotation, const WCHAR* textureTag);
	bool SetupPrefab(const WCHAR* prefabTag);
	bool SetupPrefab(uint32_t index);

	void RenderSelectedPrefab(XMFLOAT2 position);

private:
	PrefabManager();
	virtual ~PrefabManager() override = default;

	virtual void destroy() override;

public:
	Actor* SelectedPrefab;

private:
	static PrefabManager* mInstance;

	std::unordered_map<const WCHAR*, Actor*> mPrefabs;
	std::vector<Actor*> mPrefabVec;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};