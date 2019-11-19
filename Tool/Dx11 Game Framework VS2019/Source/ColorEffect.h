#pragma once
#include "Component.h"

template <typename T>
class ConstantBuffer;

class ColorEffect final : public Component
{
public:
	static ColorEffect* Create();
	bool Clone(Component** outInstance, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;
	virtual void ReadyRender() const override;
	virtual void AfterRender() const override;

	inline void SetAlphaBlendingMode(bool mode);
	void SetAlpha(float alpha);

	virtual bool CloneForPrefab(Component** outInstance) override;

protected:
	ColorEffect();
	explicit ColorEffect(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	explicit ColorEffect(const ColorEffect& other);
	virtual ~ColorEffect() override = default;

	bool initialize();

	virtual void destroy() override;

protected:
	ConstantBuffer<CB_PS_COLOR>* mCBufferColor;

	bool mbAlphaBlendingMode;
	ID3D11BlendState* mBlendState;
	ID3D11BlendState* mNoBlendState;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};

inline void ColorEffect::SetAlphaBlendingMode(bool mode)
{
	mbAlphaBlendingMode = mode;
}