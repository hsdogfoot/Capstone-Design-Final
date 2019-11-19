#include "stdafx.h"
#include "Texture.h"

bool Texture::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, Resource** outInstance)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(path != nullptr);

	Texture* instance = new Texture(deviceContext, path);
	if (!instance->initialize(device, path))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void Texture::ReadyRender() const
{
	mDeviceContext->PSSetShaderResources(0, 1, &mTexture);
}

const WCHAR* Texture::GetTexturePath() const
{
	return mPath;
}

Texture::Texture(ID3D11DeviceContext* deviceContext, const WCHAR* path)
	: Resource()
	, mPath(path)
	, mTexture(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

bool Texture::initialize(ID3D11Device* device, const WCHAR* path)
{
	HRESULT hResult = CreateWICTextureFromFile(device, path, nullptr, &mTexture);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

void Texture::destroy()
{
	ReleaseCOM(mTexture);

	ReleaseCOM(mDeviceContext);
}
