#include "stdafx.h"
#include "Renderer.h"

#include "Actor.h"
#include "ConstantBuffer.h"

Renderer* Renderer::Create()
{
	return new Renderer();
}

bool Renderer::Clone(Component** outInstance, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* vsTag, const WCHAR* psTag, const WCHAR* textureTag)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(vsTag != nullptr);
	Assert(psTag != nullptr);
	Assert(textureTag != nullptr);

	Renderer* instance = new Renderer(device, deviceContext);
	if (!instance->initialize(vsTag, psTag, textureTag))
	{
		instance->Release();

		return false;
	}
	
	*outInstance = instance;

	return true;
}

void Renderer::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);
	static_cast<Actor*>(Owner)->SetRenderer(this);
}

uint8_t Renderer::Update()
{
	return 0;
}

void Renderer::ReadyRender() const
{
	VShader->ReadyRender();
	PShader->ReadyRender();
	mBuffer->ReadyRender();
	mTexture->ReadyRender();

	mCBufferWVP->Data.World = XMMatrixTranspose(static_cast<Actor*>(Owner)->GetTransform()->World);
	mCBufferWVP->Data.View = XMMatrixTranspose(CameraManager::GetInstance()->MainCamera->GetView());
	mCBufferWVP->Data.Projection = XMMatrixTranspose(CameraManager::GetInstance()->MainCamera->GetProjection());
	mCBufferWVP->ApplyChanges();

	mDeviceContext->VSSetConstantBuffers(0, 1, &mCBufferWVP->Buffer);
}

void Renderer::Render() const
{
	mDeviceContext->DrawIndexed(mBuffer->GetBufferSize(), 0, 0);
}

const WCHAR* Renderer::GetTexturePath() const
{
	return mTexture->GetTexturePath();
}

bool Renderer::CloneForPrefab(Component** outInstance)
{
	Renderer* clone = new Renderer(*this);

	if (!ConstantBuffer<CB_VS_WVP>::Create(clone->mDevice, clone->mDeviceContext, &clone->mCBufferWVP))
	{
		ErrorLog("Constant buffer create failed.");

		return false;
	}

	*outInstance = clone;

	return true;
}

Renderer::Renderer()
	: Component()
	, VShader(nullptr)
	, PShader(nullptr)
	, mBuffer(nullptr)
	, mTexture(nullptr)
	, mCBufferWVP(nullptr)
	, mDevice(nullptr)
	, mDeviceContext(nullptr)
{
}

Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: Component()
	, VShader(nullptr)
	, PShader(nullptr)
	, mBuffer(nullptr)
	, mTexture(nullptr)
	, mCBufferWVP(nullptr)
	, mDevice(device)
	, mDeviceContext(deviceContext)
{
	mDevice->AddRef();
	mDeviceContext->AddRef();
}

Renderer::Renderer(const Renderer& other)
	: Component()
	, VShader(other.VShader)
	, PShader(other.PShader)
	, mBuffer(other.mBuffer)
	, mTexture(other.mTexture)
	, mCBufferWVP(nullptr)
	, mDevice(other.mDevice)
	, mDeviceContext(other.mDeviceContext)
{
	VShader->AddRefCount();
	PShader->AddRefCount();
	mBuffer->AddRefCount();
	mTexture->AddRefCount();
	mDevice->AddRef();
	mDeviceContext->AddRef();
}

bool Renderer::initialize(const WCHAR* vsTag, const WCHAR* psTag, const WCHAR* textureTag)
{
	Resource* clone = nullptr;

	if (!ResourceFactory::GetInstance()->GetClone(vsTag, &clone))
	{
		ErrorLog("VertexShader clone failed.");

		return false;
	}
	VShader = static_cast<VertexShader*>(clone);
	VShader->AddRefCount();

	if (!ResourceFactory::GetInstance()->GetClone(psTag, &clone))
	{
		ErrorLog("PixelShader clone failed.");

		return false;
	}
	PShader = static_cast<PixelShader*>(clone);
	PShader->AddRefCount();

	if (!ResourceFactory::GetInstance()->GetClone(L"RectBuffer", &clone))
	{
		ErrorLog("RectBuffer clone failed.");

		return false;
	}
	mBuffer = static_cast<RectBuffer*>(clone);
	mBuffer->AddRefCount();

	if (!ResourceFactory::GetInstance()->GetClone(textureTag, &clone))
	{
		ErrorLog("Texture clone failed.");

		return false;
	}
	mTexture = static_cast<Texture*>(clone);
	mTexture->AddRefCount();

	if (!ConstantBuffer<CB_VS_WVP>::Create(mDevice, mDeviceContext, &mCBufferWVP))
	{
		ErrorLog("Constant buffer create failed.");

		return false;
	}

	return true;
}

void Renderer::destroy()
{
	ReleaseBase(VShader);
	ReleaseBase(PShader);
	ReleaseBase(mBuffer);
	ReleaseBase(mTexture);
	ReleaseBase(mCBufferWVP);

	ReleaseCOM(mDevice);
	ReleaseCOM(mDeviceContext);

	Component::destroy();
}
