#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
	static Transform* Create();
	bool Clone(Component** outInstance, XMFLOAT2 scale, XMFLOAT2 rotation, XMFLOAT2 position);

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;

	virtual bool CloneForPrefab(Component** outInstance) override;

private:
	Transform();
	explicit Transform(XMFLOAT2 scale, XMFLOAT2 rotation, XMFLOAT2 position);
	explicit Transform(const Transform& other);
	virtual ~Transform() override = default;

	void* operator new(size_t size);
	void operator delete(void* memory);

	virtual void destroy() override;

public:
	XMVECTOR Scale;
	XMVECTOR Rotation;
	XMVECTOR Position;

	XMMATRIX World;
};