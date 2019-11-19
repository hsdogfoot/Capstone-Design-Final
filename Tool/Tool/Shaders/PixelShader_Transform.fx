sampler2D samplerInput : register(s0);
float factor : register(c0);
float lrSwap : register(c1);
float tdSwap : register(c2);

float4 main(float2 uv : TEXCOORD) : COLOR
{
    float2 transformedUV;
    float2 lrSwappedUV;
    float2 tdSwappedUV;

    if (abs(lrSwap - 1.0) < 0.1)
    {
        lrSwappedUV.x = 1 - uv.x;
        lrSwappedUV.y = uv.y;
    }
    else
    {
        lrSwappedUV.x = uv.x;
        lrSwappedUV.y = uv.y;
    }

    if (abs(tdSwap - 1.0) < 0.1)
    {
        tdSwappedUV.x = lrSwappedUV.x;
        tdSwappedUV.y = 1 - lrSwappedUV.y;
    }
    else
    {
        tdSwappedUV.x = lrSwappedUV.x;
        tdSwappedUV.y = lrSwappedUV.y;
    }

    if (abs(factor - 90.0) < 0.1)
    {
        transformedUV.x = 1 - tdSwappedUV.y;
        transformedUV.y = tdSwappedUV.x;
    }
    else if (abs(factor - 180.0) < 0.1)
    {
        transformedUV.x = 1 - tdSwappedUV.x;
        transformedUV.y = 1 - tdSwappedUV.y;
    }
    else if (abs(factor - 270.0) < 0.1)
    {
        transformedUV.x = tdSwappedUV.y;
        transformedUV.y = 1 - tdSwappedUV.x;
    }
    else if (abs(factor - 0.0) < 0.1)
    {
        transformedUV.x = tdSwappedUV.x;
        transformedUV.y = tdSwappedUV.y;
    }

    return tex2D(samplerInput, transformedUV);
}