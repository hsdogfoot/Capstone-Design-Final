#include "stdafx.h"
#include "VertexShader.h"

D3D11_INPUT_ELEMENT_DESC VertexShader::LAYOUT_DESC_XYZ_TEXTURE[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC VertexShader::LAYOUT_DESC_XY_COLOR[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

bool VertexShader::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements, Resource** outInstance)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(path != nullptr);

	VertexShader* instance = new VertexShader(deviceContext);
	if (!instance->initialize(device, path, inputElementDesc, numElements))
	{
		instance->Release();

		return false;
	}

	*outInstance = instance;

	return true;
}

void VertexShader::ReadyRender() const
{
	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->VSSetShader(mShader, nullptr, 0);
}

VertexShader::VertexShader(ID3D11DeviceContext* deviceContext)
	: Shader(deviceContext)
	, mShader(nullptr)
	, mInputLayout(nullptr)
{
}

bool VertexShader::initialize(ID3D11Device* device, const WCHAR* path, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements)
{
	HRESULT hResult = D3DReadFileToBlob(path, &mShaderBuffer);
	if (FAILED(hResult))
	{
		HRAndPathLog(hResult, path);

		return false;
	}

	hResult = device->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = device->CreateInputLayout(inputElementDesc, numElements, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

void VertexShader::destroy()
{
	ReleaseCOM(mShader);
	ReleaseCOM(mInputLayout);

	Shader::destroy();
}
