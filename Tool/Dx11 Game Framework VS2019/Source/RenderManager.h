#pragma once
#include "Base.h"

class Actor;

class RenderManager final : public Base
{
public:
	enum { MaxOffsetLevel = 3 };

public:
	static RenderManager* GetInstance();
	static uint32_t DestroyInstance();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t gridLevel, uint32_t gridOffsetLevel, bool bGridMode = false);

	void RegisterActor(Actor* actor);

	void ToggleGrid();
	void WidenGrid();
	void NarrowGrid();
	void RenderGrid();

	void Render();

private:
	RenderManager();
	virtual ~RenderManager() override = default;

	bool initializeGridData(ID3D11Device* device);

	virtual void destroy() override;

public:
	std::vector<Actor*> Actors;

private:
	static RenderManager* mInstance;

	GridBuffer* mGridBuffer;
	VertexShader* mGridVShader;
	PixelShader* mGridPShader;
	
	static float GRID_OFFSET[];

	bool mbGridMode;
	uint32_t mGridLevel;
	uint32_t mGridOffsetLevel;	

	ConstantBuffer<CB_VS_WVP>* mCBufferWVP;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};