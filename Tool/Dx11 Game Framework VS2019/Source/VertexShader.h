#pragma once
#include "Shader.h"

class VertexShader final : public Shader
{
public:
	static bool Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements, Resource** outInstance);

	virtual void ReadyRender() const override;

protected:
	explicit VertexShader(ID3D11DeviceContext* deviceContext);
	virtual ~VertexShader() override = default;

	bool initialize(ID3D11Device* device, const WCHAR* path, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements);

	virtual void destroy() override;

public:
	static D3D11_INPUT_ELEMENT_DESC LAYOUT_DESC_XYZ_TEXTURE[];
	static D3D11_INPUT_ELEMENT_DESC LAYOUT_DESC_XY_COLOR[];

protected:
	ID3D11VertexShader* mShader;
	ID3D11InputLayout* mInputLayout;
};