#pragma once
#include "Resource.h"

template <typename T>
class VertexBuffer;
class IndexBuffer;

struct Vertex;

class GridBuffer final : public Resource
{
public:
	static bool Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t gridLevel, float gridOffset, Resource** outInstance);

	virtual void ReadyRender() const override;

	UINT GetBufferSize() const;

private:
	explicit GridBuffer(ID3D11DeviceContext* deviceContext);
	virtual ~GridBuffer() override = default;

	bool initialize(ID3D11Device* device, uint32_t gridLevel, float gridOffset);
	bool createVertexBuffer(ID3D11Device* device, uint32_t gridLevel, float gridOffset);
	bool createIndexBuffer(ID3D11Device* device, uint32_t gridLevel, float gridOffset);

	virtual void destroy() override;

private:
	VertexBuffer<Vertex_Color>* mVBuffer;
	IndexBuffer* mIBuffer;

	ID3D11DeviceContext* mDeviceContext;
};