#include "stdafx.h"
#include "RenderManager.h"

#include "Actor.h"
#include "ConstantBuffer.h"

RenderManager* RenderManager::mInstance = nullptr;

float RenderManager::GRID_OFFSET[] = { 1.0f, 0.5f, 0.25f, 0.1f };

RenderManager* RenderManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new RenderManager();
	}

	return mInstance;
}

uint32_t RenderManager::DestroyInstance()
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

bool RenderManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t gridLevel, uint32_t gridOffsetLevel, bool bGridMode)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	if (gridLevel < 1)
	{
		gridLevel = 1;
	}

	if (gridOffsetLevel > MaxOffsetLevel)
	{
		gridOffsetLevel = MaxOffsetLevel;
	}

	mDevice = device;
	mDevice->AddRef();

	mDeviceContext = deviceContext;
	mDeviceContext->AddRef();

	mbGridMode = bGridMode;
	mGridLevel = gridLevel;
	mGridOffsetLevel = gridOffsetLevel;

	if (!initializeGridData(device))
	{
		return false;
	}

	return true;
}

void RenderManager::RegisterActor(Actor* actor)
{
	Assert(actor != nullptr);

	Actors.push_back(actor);
	actor->AddRefCount();
}

void RenderManager::ToggleGrid()
{
	mbGridMode = !mbGridMode;
}

void RenderManager::WidenGrid()
{
	if (mGridOffsetLevel == 0)
	{
		return;
	}

	ReleaseBase(mGridBuffer);

	--mGridOffsetLevel;

	Resource* prototype = nullptr;
	if (!GridBuffer::Create(mDevice, mDeviceContext, mGridLevel, GRID_OFFSET[mGridOffsetLevel], &prototype))
	{
		ErrorLog("Grid buffer create failed.");

		return;
	}

	mGridBuffer = static_cast<GridBuffer*>(prototype);
}

void RenderManager::NarrowGrid()
{
	if (mGridOffsetLevel == 3)
	{
		return;
	}

	ReleaseBase(mGridBuffer);

	++mGridOffsetLevel;

	Resource* prototype = nullptr;
	if (!GridBuffer::Create(mDevice, mDeviceContext, mGridLevel, GRID_OFFSET[mGridOffsetLevel], &prototype))
	{
		ErrorLog("Grid buffer create failed.");

		return;
	}

	mGridBuffer = static_cast<GridBuffer*>(prototype);
}

void RenderManager::RenderGrid()
{
	if (!mbGridMode)
	{
		return;
	}

	mGridBuffer->ReadyRender();
	mGridVShader->ReadyRender();
	mGridPShader->ReadyRender();

	mCBufferWVP->Data.World = XMMatrixIdentity();
	mCBufferWVP->Data.View = XMMatrixTranspose(CameraManager::GetInstance()->MainCamera->GetView());
	mCBufferWVP->Data.Projection = XMMatrixTranspose(CameraManager::GetInstance()->MainCamera->GetProjection());
	mCBufferWVP->ApplyChanges();

	mDeviceContext->VSSetConstantBuffers(0, 1, &mCBufferWVP->Buffer);

	mDeviceContext->DrawIndexed(mGridBuffer->GetBufferSize(), 0, 0);
}

void RenderManager::Render()
{
	for (auto actor : Actors)
	{
		actor->ReadyRender();
		actor->Render();
		actor->AfterRender();
		actor->Release();
	}

	Actors.clear();
}

RenderManager::RenderManager()
	: Base()
	, mGridBuffer(nullptr)
	, mGridVShader(nullptr)
	, mGridPShader(nullptr)
	, mbGridMode(false)	
	, mGridLevel(0)
	, mGridOffsetLevel(0)
	, mCBufferWVP(nullptr)
	, mDevice(nullptr)
	, mDeviceContext(nullptr)
{
}

bool RenderManager::initializeGridData(ID3D11Device* device)
{
	Resource* prototype = nullptr;

	if (!GridBuffer::Create(device, mDeviceContext, mGridLevel, GRID_OFFSET[mGridOffsetLevel], &prototype))
	{
		ErrorLog("Grid buffer create failed.");

		return false;
	}
	mGridBuffer = static_cast<GridBuffer*>(prototype);

	if (!VertexShader::Create(device, mDeviceContext, L"Resources\\Shaders\\VS_Grid.cso", VertexShader::LAYOUT_DESC_XY_COLOR, 2, &prototype))
	{
		ErrorLog("Grid vertex shader create failed.");

		return false;
	}
	mGridVShader = static_cast<VertexShader*>(prototype);

	if (!PixelShader::Create(device, mDeviceContext, L"Resources\\Shaders\\PS_Grid.cso", &prototype))
	{
		ErrorLog("Grid pixel shader create failed.");

		return false;
	}
	mGridPShader = static_cast<PixelShader*>(prototype);

	if (!ConstantBuffer<CB_VS_WVP>::Create(device, mDeviceContext, &mCBufferWVP))
	{
		ErrorLog("Constant buffer create failed.");

		return false;
	}

	return true;
}

void RenderManager::destroy()
{
	for (auto actor : Actors)
	{
		actor->Release();
	}

	ReleaseBase(mGridBuffer);
	ReleaseBase(mGridVShader);
	ReleaseBase(mGridPShader);

	ReleaseBase(mCBufferWVP);

	ReleaseCOM(mDevice);
	ReleaseCOM(mDeviceContext);
}

