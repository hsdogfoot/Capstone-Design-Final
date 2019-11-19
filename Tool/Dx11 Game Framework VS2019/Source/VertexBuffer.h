#pragma once
#include "Base.h"

#include "stdafx.h"

template <typename Type>
class VertexBuffer final : public Base
{
public:
	static bool Create(ID3D11Device* device, Type* data, UINT numElements, VertexBuffer<Type>** outInstance);

private:
	VertexBuffer(UINT numElements);
	virtual ~VertexBuffer() override = default;

	bool initialize(ID3D11Device* device, Type* data);

	virtual void destroy() override;

public:
	ID3D11Buffer* Buffer;
	UINT BufferSize;
	std::unique_ptr<UINT> Stride;
};

template<typename Type>
bool VertexBuffer<Type>::Create(ID3D11Device* device, Type* data, UINT numElements, VertexBuffer<Type>** outInstance)
{
	Assert(device != nullptr);
	Assert(data != nullptr);

	VertexBuffer<Type>* instance = new VertexBuffer<Type>(numElements);
	if (!instance->initialize(device, data))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

template<typename Type>
VertexBuffer<Type>::VertexBuffer(UINT numElements)
	: Buffer(nullptr)
	, BufferSize(numElements)
{
}

template<typename Type>
bool VertexBuffer<Type>::initialize(ID3D11Device* device, Type* data)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Type) * BufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourceData.pSysMem = data;

	HRESULT hResult = device->CreateBuffer(&bufferDesc, &subResourceData, &Buffer);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	Stride = std::make_unique<UINT>(sizeof(Type));

	return true;
}

template<typename Type>
void VertexBuffer<Type>::destroy()
{
	ReleaseCOM(Buffer);
}
