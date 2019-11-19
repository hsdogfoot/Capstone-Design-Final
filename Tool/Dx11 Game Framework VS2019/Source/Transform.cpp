#include "stdafx.h"
#include "Transform.h"

Transform* Transform::Create()
{
	return new Transform();
}

bool Transform::Clone(Component** outInstance, XMFLOAT2 scale, XMFLOAT2 rotation, XMFLOAT2 position)
{
	*outInstance = new Transform(scale, rotation, position);

	return true;
}

void Transform::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);
	static_cast<Actor*>(Owner)->SetTransform(this);
}

uint8_t Transform::Update()
{
	World = XMMatrixScalingFromVector(Scale) * XMMatrixRotationZ(XMVectorGetX(Rotation)) * XMMatrixTranslationFromVector(Position);

	return 0;
}

bool Transform::CloneForPrefab(Component** outInstance)
{
	Transform* clone = new Transform(*this);
	
	*outInstance = clone;

	return true;
}

Transform::Transform()
	: Component()
	, Scale(XMVectorZero())
	, Rotation(XMVectorZero())
	, Position(XMVectorZero())
	, World(XMMatrixIdentity())
{
}

Transform::Transform(XMFLOAT2 scale, XMFLOAT2 rotation, XMFLOAT2 position)
	: Component()
	, Scale(XMLoadFloat2(&scale))
	, Rotation(XMLoadFloat2(&rotation))
	, Position(XMLoadFloat2(&position))
	, World(XMMatrixIdentity())
{
}

Transform::Transform(const Transform& other)
	: Component()
	, Scale(other.Scale)
	, Rotation(other.Rotation)
	, Position(other.Position)
	, World(other.World)
{
}

void* Transform::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void Transform::operator delete(void* memory)
{
	_aligned_free(memory);
}

void Transform::destroy()
{
	Component::destroy();
}
