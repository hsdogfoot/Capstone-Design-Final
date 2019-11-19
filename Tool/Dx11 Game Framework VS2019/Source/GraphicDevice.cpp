#include "stdafx.h"
#include "GraphicDevice.h"

#include "AdapterReader.h"
#include "RenderManager.h"

bool GraphicDevice::Create(HWND windowHandle, int width, int height, GraphicDevice** outIstance)
{
	Assert(windowHandle != nullptr);

	GraphicDevice* instance = new GraphicDevice();
	if (!instance->initialize(windowHandle, width, height))
	{
		instance->Release();

		return false;
	}

	*outIstance = instance;

	return true;
}

void GraphicDevice::Render() const
{
	//float bgColor[] = { 1.0f, 0.75f, 0.15f, 0.0f };
	float bgColor[] = { 0.5f, 0.5f, 0.55f, 0.0f };
	DeviceContext->ClearRenderTargetView(mRenderTargetView, bgColor);
	DeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DeviceContext->RSSetState(mRasterizerState);
	DeviceContext->OMSetBlendState(mBlendState, nullptr, 0xFFFFFF);
	DeviceContext->OMSetDepthStencilState(mDepthStencilState, 0);
	DeviceContext->PSSetSamplers(0, 1, &mSamplerState);

	RenderManager::GetInstance()->RenderGrid();
	
	RenderManager::GetInstance()->Render();
	PrefabManager::GetInstance()->RenderSelectedPrefab(InputManager::GetInstance()->GetMouseAtWorld());

	mSwapChain->Present(1, 0);
}

GraphicDevice::GraphicDevice()
	: Base()
	, Device(nullptr)
	, DeviceContext(nullptr)
	, mSwapChain(nullptr)
	, mRenderTargetView(nullptr)
	, mDepthStencilView(nullptr)
	, mDepthStencilBuffer(nullptr)
	, mDepthStencilState(nullptr)
	, mRasterizerState(nullptr)
	, mSamplerState(nullptr)
	, mBlendState(nullptr)
{
}

bool GraphicDevice::initialize(HWND windowHandle, int width, int height)
{
	HRESULT hResult = CoInitialize(nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	if (!initializeDirectX(windowHandle, width, height))
	{
		return false;
	}

	return true;
}

bool GraphicDevice::initializeDirectX(HWND windowHandle, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	if (adapters.empty())
	{
		ErrorLog("GetAdapters failed.");

		return false;
	}

	UINT createFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hResult = D3D11CreateDeviceAndSwapChain(adapters[0].Adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, createFlags, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &Device, nullptr, &DeviceContext);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	ID3D11Texture2D* backBuffer;
	hResult = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = Device->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	ReleaseCOM(backBuffer);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hResult = Device->CreateTexture2D(&textureDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = Device->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	DeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hResult = Device->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	DeviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	hResult = Device->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hResult = Device->CreateSamplerState(&samplerDesc, &mSamplerState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hResult = Device->CreateBlendState(&blendDesc, &mBlendState);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

void GraphicDevice::destroy()
{
	ReleaseCOM(Device);
	ReleaseCOM(DeviceContext);

	ReleaseCOM(mSwapChain);
	ReleaseCOM(mRenderTargetView);

	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mDepthStencilState);

	ReleaseCOM(mRasterizerState);
	ReleaseCOM(mSamplerState);
	ReleaseCOM(mBlendState);

	//for (auto adapterData : AdapterReader::GetAdapters())
	//{
	//	ReleaseCOM(adapterData.Adapter);
	//}
}
