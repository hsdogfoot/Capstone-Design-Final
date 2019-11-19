#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex()
		: Position(), TexCoord() {}
	Vertex(float x, float y, float z, float u, float v)
		: Position(x, y, z), TexCoord(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};

struct Vertex_Color
{
	Vertex_Color()
		: Position(), Color() {}
	Vertex_Color(float x, float y, float r, float g, float b)
		: Position(x, y), Color(r, g, b) {}

	DirectX::XMFLOAT2 Position;
	DirectX::XMFLOAT3 Color;
};

struct CB_VS_EMPTY
{
	char padding;
};

struct CB_PS_EMPTY
{
	char padding;
};

struct CB_VS_WVP
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};

struct CB_PS_COLOR
{
	float red;
	float green;
	float blue;
	float alpha;
};