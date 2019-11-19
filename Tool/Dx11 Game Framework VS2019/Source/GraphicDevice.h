#pragma once
#include "Base.h"

class GraphicDevice final : public Base
{
public:
	static bool Create(HWND windowHandle, int width, int height, GraphicDevice** outIstance);

	void Render() const;

private:
	GraphicDevice();
	virtual ~GraphicDevice() override = default;

	bool initialize(HWND windowHandle, int width, int height);
	bool initializeDirectX(HWND windowHandle, int width, int height);

	virtual void destroy() override;

public:
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

private:
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;

	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;

	ID3D11RasterizerState* mRasterizerState;
	ID3D11SamplerState* mSamplerState;
	ID3D11BlendState* mBlendState;
};