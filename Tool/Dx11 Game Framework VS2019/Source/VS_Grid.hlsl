cbuffer mycBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float2 inPosition : POSITION;
    float3 inColor : COLOR;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float3 outColor : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    matrix wv = mul(world, view);
    matrix wvp = mul(wv, projection);

    output.outPosition = mul(float4(input.inPosition, 0.0f, 1.0f), wvp);
    output.outColor = input.inColor;

    return output;
}