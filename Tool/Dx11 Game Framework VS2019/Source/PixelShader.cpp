#include "stdafx.h"
#include "PixelShader.h"

bool PixelShader::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, Resource** outInstance)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(path != nullptr);

	PixelShader* instance = new PixelShader(deviceContext);
	if (!instance->initialize(device, path))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void PixelShader::ReadyRender() const
{
	mDeviceContext->PSSetShader(mShader, nullptr, 0);
}

PixelShader::PixelShader(ID3D11DeviceContext* deviceContext)
	: Shader(deviceContext)
	, mShader(nullptr)
{
}

bool PixelShader::initialize(ID3D11Device* device, const WCHAR* path)
{
	HRESULT hResult = D3DReadFileToBlob(path, &mShaderBuffer);
	if (FAILED(hResult))
	{
		HRAndPathLog(hResult, path);

		return false;
	}

	hResult = device->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

void PixelShader::destroy()
{
	ReleaseCOM(mShader);

	Shader::destroy();
}
