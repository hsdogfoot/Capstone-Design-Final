#include "stdafx.h"
#include "GridBuffer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

bool GridBuffer::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t gridLevel, float gridOffset, Resource** outInstance)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	GridBuffer* instance = new GridBuffer(deviceContext);
	if (!instance->initialize(device, gridLevel, gridOffset))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void GridBuffer::ReadyRender() const
{
	mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVBuffer->Buffer, mVBuffer->Stride.get(), &offset);
	mDeviceContext->IASetIndexBuffer(mIBuffer->Buffer, DXGI_FORMAT_R32_UINT, 0);
}

UINT GridBuffer::GetBufferSize() const
{
	return mIBuffer->BufferSize;
}

GridBuffer::GridBuffer(ID3D11DeviceContext* deviceContext)
	: Resource()
	, mVBuffer(nullptr)
	, mIBuffer(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

bool GridBuffer::initialize(ID3D11Device* device, uint32_t gridLevel, float gridOffset)
{
	if (!createVertexBuffer(device, gridLevel, gridOffset))
	{
		return false;
	}

	if (!createIndexBuffer(device, gridLevel, gridOffset))
	{
		return false;
	}

	return true;
}

bool GridBuffer::createVertexBuffer(ID3D11Device* device, uint32_t gridLevel, float gridOffset)
{
	int count = 8 * gridLevel - 4;

	Vertex_Color* vertices = new Vertex_Color[count];

	for (int i = -count / 8; i <= count / 8; ++i)
	{
		vertices[i + count * 1 / 8] = Vertex_Color(i * gridOffset, gridLevel * gridOffset * +1.0f, 0.0f, 0.0f, 1.0f);
		vertices[i + count * 3 / 8] = Vertex_Color(i * gridOffset, gridLevel * gridOffset * -1.0f, 0.0f, 0.0f, 1.0f);
		vertices[i + count * 5 / 8] = Vertex_Color(gridLevel * gridOffset * -1.0f, i * gridOffset, 0.0f, 0.0f, 1.0f);
		vertices[i + count * 7 / 8] = Vertex_Color(gridLevel * gridOffset * +1.0f, i * gridOffset, 0.0f, 0.0f, 1.0f);
	}

	if (!VertexBuffer<Vertex_Color>::Create(device, vertices, count, &mVBuffer))
	{
		ErrorLog("VertexBuffer create failed.");

		return false;
	}

	return true;
}

bool GridBuffer::createIndexBuffer(ID3D11Device* device, uint32_t gridLevel, float gridOffset)
{
	int count = 8 * gridLevel - 4;

	DWORD* indices = new DWORD[count];

	for (int i = 0; i < count / 4; ++i)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + count * 2 / 8;

		indices[i * 2 + count / 2] = i + count / 2;
		indices[i * 2 + count / 2 + 1] = i + count / 2 + count / 4;
	}

	if (!IndexBuffer::Create(device, indices, count, &mIBuffer))
	{
		ErrorLog("IndexBuffer create failed.");

		return false;
	}

	return true;
}

void GridBuffer::destroy()
{
	ReleaseBase(mVBuffer);
	ReleaseBase(mIBuffer);

	ReleaseCOM(mDeviceContext)
}
