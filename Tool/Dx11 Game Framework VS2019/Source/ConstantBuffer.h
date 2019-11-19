#pragma once
#include "Base.h"

#include "stdafx.h"

template <typename Type>
class ConstantBuffer final : public Base
{
public:
	static bool Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<Type>** outInstance);

	inline void SetData(void* data);
	bool ApplyChanges() const;

private:
	ConstantBuffer(ID3D11DeviceContext* deviceContext);
	virtual ~ConstantBuffer() override = default;

	inline void* operator new(size_t size);
	inline void operator delete(void* memory);

	bool initialize(ID3D11Device* device);

	virtual void destroy() override;

public:
	ID3D11Buffer* Buffer;
	Type Data;

private:
	ID3D11DeviceContext* mDeviceContext;
};

template<typename Type>
bool ConstantBuffer<Type>::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<Type>** outInstance)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	ConstantBuffer<Type>* instance = new ConstantBuffer<Type>(deviceContext);
	if (!instance->initialize(device))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

template<typename Type>
inline void ConstantBuffer<Type>::SetData(void* data)
{
	Data = *(static_cast<Type*>(data));
}

template<typename Type>
bool ConstantBuffer<Type>::ApplyChanges() const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hResult = mDeviceContext->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	CopyMemory(mappedResource.pData, &Data, sizeof(Type));
	mDeviceContext->Unmap(Buffer, 0);

	return true;
}

template<typename Type>
ConstantBuffer<Type>::ConstantBuffer(ID3D11DeviceContext* deviceContext)
	: Buffer(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();

	ZeroMemory(&Data, sizeof(Type));
}

template<typename Type>
inline void* ConstantBuffer<Type>::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

template<typename Type>
inline void ConstantBuffer<Type>::operator delete(void* memory)
{
	_aligned_free(memory);
}

template<typename Type>
bool ConstantBuffer<Type>::initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Type) + (16 - (sizeof(Type) % 16)));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hResult = device->CreateBuffer(&bufferDesc, nullptr, &Buffer);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

template<typename Type>
void ConstantBuffer<Type>::destroy()
{
	ReleaseCOM(Buffer);

	ReleaseCOM(mDeviceContext);
}
