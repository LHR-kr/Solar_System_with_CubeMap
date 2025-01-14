#include "Common.hlsli" // 쉐이더에서도 include 사용 가능

TextureCube g_textureCube0 : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{

    return g_textureCube0.Sample(g_sampler, input.posWorld.xyz);
}