#pragma once
#include "Shader.h"

class PixelShader final : public Shader
{
public:
	static bool Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, Resource** outInstance);

	virtual void ReadyRender() const override;

protected:
	explicit PixelShader(ID3D11DeviceContext* deviceContext);
	virtual ~PixelShader() override = default;

	bool initialize(ID3D11Device* device, const WCHAR* path);

	virtual void destroy() override;

protected:
	ID3D11PixelShader* mShader;
};