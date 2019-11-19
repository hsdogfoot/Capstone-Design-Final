#include "stdafx.h"
#include "Shader.h"

Shader::Shader(ID3D11DeviceContext* deviceContext)
	: Resource()
	, mShaderBuffer(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

void Shader::destroy()
{
	ReleaseCOM(mShaderBuffer);

	ReleaseCOM(mDeviceContext);
}