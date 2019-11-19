#pragma once
#include "Resource.h"

class Shader abstract : public Resource
{
public:
	explicit Shader(ID3D11DeviceContext* deviceContext);
	virtual ~Shader() override = default;

	virtual void ReadyRender() const override = 0;

protected:
	virtual void destroy() override;

protected:
	ID3D10Blob* mShaderBuffer;

	ID3D11DeviceContext* mDeviceContext;
};