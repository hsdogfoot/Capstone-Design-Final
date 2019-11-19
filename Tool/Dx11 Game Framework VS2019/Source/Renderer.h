#pragma once
#include "Component.h"

template <typename T>
class ConstantBuffer;

class Renderer final : public Component
{
public:
	static Renderer* Create();
	bool Clone(Component** outInstance, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* vsTag, const WCHAR* psTag, const WCHAR* textureTag);

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;
	virtual void ReadyRender() const override;
	void Render() const;

	const WCHAR* GetTexturePath() const;

	virtual bool CloneForPrefab(Component** outInstance) override;

protected:
	Renderer();
	explicit Renderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	explicit Renderer(const Renderer& other);
	virtual ~Renderer() override = default;

	bool initialize(const WCHAR* vsTag, const WCHAR* psTag, const WCHAR* textureTag);

	virtual void destroy() override;

public:
	VertexShader* VShader;
	PixelShader* PShader;

protected:
	RectBuffer* mBuffer;
	Texture* mTexture;

	ConstantBuffer<CB_VS_WVP>* mCBufferWVP;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};