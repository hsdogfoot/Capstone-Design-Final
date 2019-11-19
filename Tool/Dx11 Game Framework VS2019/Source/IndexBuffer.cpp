#include "stdafx.h"
#include "IndexBuffer.h"

bool IndexBuffer::Create(ID3D11Device* device, DWORD* data, UINT numElements, IndexBuffer** outInstance)
{
	Assert(device != nullptr);
	Assert(data != nullptr);

	IndexBuffer* instance = new IndexBuffer(numElements);
	if (!instance->initialize(device, data))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

IndexBuffer::IndexBuffer(UINT numElements)
	: Buffer(nullptr)
	, BufferSize(numElements)
{
}

bool IndexBuffer::initialize(ID3D11Device* device, DWORD* data)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DWORD) * BufferSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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

	return true;
}

void IndexBuffer::destroy()
{
	ReleaseCOM(Buffer);
}
