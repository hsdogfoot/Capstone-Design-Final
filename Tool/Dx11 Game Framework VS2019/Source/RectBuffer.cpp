#include "stdafx.h"
#include "RectBuffer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

Vertex RectBuffer::RectVertices[] = 
{
		Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, +0.5f, 0.0f, 0.0f, 0.0f),
		Vertex(+0.5f, +0.5f, 0.0f, 1.0f, 0.0f),
		Vertex(+0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
};

DWORD RectBuffer::RectIndices[] =
{
	0, 1, 2,
	0, 2, 3,
};

bool RectBuffer::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Resource** outInstance)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	RectBuffer* instance = new RectBuffer(deviceContext);
	if (!instance->initialize(device))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void RectBuffer::ReadyRender() const
{
	mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVBuffer->Buffer, mVBuffer->Stride.get(), &offset);
	mDeviceContext->IASetIndexBuffer(mIBuffer->Buffer, DXGI_FORMAT_R32_UINT, 0);
}

UINT RectBuffer::GetBufferSize() const
{
	return mIBuffer->BufferSize;
}

RectBuffer::RectBuffer(ID3D11DeviceContext* deviceContext)
	: Resource()
	, mVBuffer(nullptr)
	, mIBuffer(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

bool RectBuffer::initialize(ID3D11Device* device)
{
	if (!createVertexBuffer(device))
	{
		return false;
	}

	if (!createIndexBuffer(device))
	{
		return false;
	}

	return true;
}

bool RectBuffer::createVertexBuffer(ID3D11Device* device)
{
	if (!VertexBuffer<Vertex>::Create(device, RectVertices, ARRAYSIZE(RectVertices), &mVBuffer))
	{
		ErrorLog("VertexBuffer create failed.");

		return false;
	}

	return true;
}

bool RectBuffer::createIndexBuffer(ID3D11Device* device)
{
	if (!IndexBuffer::Create(device, RectIndices, ARRAYSIZE(RectIndices), &mIBuffer))
	{
		ErrorLog("IndexBuffer create failed.");

		return false;
	}

	return true;
}

void RectBuffer::destroy()
{
	ReleaseBase(mVBuffer);
	ReleaseBase(mIBuffer);

	ReleaseCOM(mDeviceContext)
}
