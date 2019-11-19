#pragma once
#include "Base.h"

class IndexBuffer final : public Base
{
public:
	static bool Create(ID3D11Device* device, DWORD* data, UINT numElements, IndexBuffer** outInstance);

private:
	explicit IndexBuffer(UINT numElements);
	virtual ~IndexBuffer() override = default;

	bool initialize(ID3D11Device* device, DWORD* data);

	virtual void destroy() override;

public:
	ID3D11Buffer* Buffer;
	UINT BufferSize;
};