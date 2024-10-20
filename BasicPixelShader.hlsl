#include "Common.hlsli" // 쉐이더에서도 include 사용 가능

// TODO:
Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer BasicPixelConstantBuffer : register(b0)
{
    float3 eyeWorld;
    bool useTexture;
    Material material;
    Light light;
    float3 rimColor;
    float rimPower;
    float rimStrength;
    bool useSmoothstep;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 toEye = normalize(eyeWorld - input.posWorld);

    float3 color = float3(0.4, 0.4, 0.4);
    color += ComputePointLight(light, material, input.posWorld, input.normalWorld, toEye);
    
    return float4(color, 1.0) * g_texture.Sample(g_sampler, input.texcoord);
}
