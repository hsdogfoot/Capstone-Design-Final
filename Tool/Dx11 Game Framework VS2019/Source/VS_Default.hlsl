cbuffer mycBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float3 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float3 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    matrix wv = mul(world, view);
    matrix wvp = mul(wv, projection);

    output.outPosition = mul(float4(input.inPosition, 1.0f), wvp);
    output.outTexCoord = input.inTexCoord;

    return output;
}