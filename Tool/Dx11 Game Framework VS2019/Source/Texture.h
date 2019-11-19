#pragma once
#include "Resource.h"

class Texture final : public Resource
{
public:
	static bool Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, Resource** outInstance);

	virtual void ReadyRender() const override;

	const WCHAR* GetTexturePath() const;

private:
	explicit Texture(ID3D11DeviceContext* deviceContext, const WCHAR* path);
	virtual ~Texture() override = default;

	bool initialize(ID3D11Device* device, const WCHAR* path);

	virtual void destroy() override;

private:
	const WCHAR* mPath;
	ID3D11ShaderResourceView* mTexture;

	ID3D11DeviceContext* mDeviceContext;
};