#include "stdafx.h"
#include "ColorEffect.h"

#include "ConstantBuffer.h"

ColorEffect* ColorEffect::Create()
{
	return new ColorEffect();
}

bool ColorEffect::Clone(Component** outInstance, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	ColorEffect* instance = new ColorEffect(device, deviceContext);
	if (!instance->initialize())
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void ColorEffect::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);
	static_cast<Actor*>(Owner)->SetColorEffect(this);
}

uint8_t ColorEffect::Update()
{
	return 0;
}

void ColorEffect::ReadyRender() const
{
	if (mbAlphaBlendingMode)
	{
		mDeviceContext->OMSetBlendState(mBlendState, nullptr, 0xFFFFFF);
	}

	mCBufferColor->ApplyChanges();

	mDeviceContext->PSSetConstantBuffers(0, 1, &mCBufferColor->Buffer);
}

void ColorEffect::AfterRender() const
{
	if (mbAlphaBlendingMode)
	{
		mDeviceContext->OMSetBlendState(mNoBlendState, nullptr, 0xFFFFFF);
	}
}

void ColorEffect::SetAlpha(float alpha)
{
	mCBufferColor->Data.alpha = alpha;
}

bool ColorEffect::CloneForPrefab(Component** outInstance)
{
	ColorEffect* clone = new ColorEffect(*this);

	if (!ConstantBuffer<CB_PS_COLOR>::Create(clone->mDevice, clone->mDeviceContext, &clone->mCBufferColor))
	{
		ErrorLog("Constant buffer create failed.");

		return false;
	}

	clone->mCBufferColor->Data.alpha = 1.0f;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hResult = clone->mDevice->CreateBlendState(&blendDesc, &clone->mBlendState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hResult = clone->mDevice->CreateBlendState(&blendDesc, &clone->mNoBlendState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	*outInstance = clone;

	return true;
}

ColorEffect::ColorEffect()
	: Component()
	, mCBufferColor(nullptr)
	, mbAlphaBlendingMode(false)
	, mBlendState(nullptr)
	, mNoBlendState(nullptr)
	, mDevice(nullptr)
	, mDeviceContext(nullptr)
{
}

ColorEffect::ColorEffect(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: Component()
	, mCBufferColor(nullptr)
	, mbAlphaBlendingMode(false)
	, mBlendState(nullptr)
	, mNoBlendState(nullptr)
	, mDevice(device)
	, mDeviceContext(deviceContext)
{
	mDevice->AddRef();
	mDeviceContext->AddRef();
}

ColorEffect::ColorEffect(const ColorEffect& other)
	: Component()
	, mCBufferColor(nullptr)
	, mbAlphaBlendingMode(other.mbAlphaBlendingMode)
	, mBlendState(nullptr)
	, mNoBlendState(nullptr)
	, mDevice(other.mDevice)
	, mDeviceContext(other.mDeviceContext)
{
	mDevice->AddRef();
	mDeviceContext->AddRef();
}

bool ColorEffect::initialize()
{
	if (!ConstantBuffer<CB_PS_COLOR>::Create(mDevice, mDeviceContext, &mCBufferColor))
	{
		ErrorLog("Constant buffer create failed.");

		return false;
	}

	mCBufferColor->Data.alpha = 1.0f;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hResult = mDevice->CreateBlendState(&blendDesc, &mBlendState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hResult = mDevice->CreateBlendState(&blendDesc, &mNoBlendState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

void ColorEffect::destroy()
{
	ReleaseBase(mCBufferColor);
	
	ReleaseCOM(mBlendState);
	ReleaseCOM(mNoBlendState);

	ReleaseCOM(mDevice);
	ReleaseCOM(mDeviceContext);

	Component::destroy();
}
