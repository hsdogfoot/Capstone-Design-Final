#pragma once
#include "Resource.h"

template <typename T>
class VertexBuffer;
class IndexBuffer;

struct Vertex;

class RectBuffer final : public Resource
{
public:
	static bool Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Resource** outInstance);

	virtual void ReadyRender() const override;

	UINT GetBufferSize() const;

private:
	explicit RectBuffer(ID3D11DeviceContext* deviceContext);
	virtual ~RectBuffer() override = default;

	bool initialize(ID3D11Device* device);
	bool createVertexBuffer(ID3D11Device* device);
	bool createIndexBuffer(ID3D11Device* device);

	virtual void destroy() override;
	
public:
	static Vertex RectVertices[];
	static DWORD RectIndices[];

private:
	VertexBuffer<Vertex>* mVBuffer;
	IndexBuffer* mIBuffer;

	ID3D11DeviceContext* mDeviceContext;
};