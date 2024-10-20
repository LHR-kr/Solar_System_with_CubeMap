#include "Common.hlsli"

cbuffer BasicVertexModelConstantBuffer : register(b0)
{
    matrix model;
    matrix invTranspose;
};
cbuffer BasicVertexViewProjConstantBuffer : register(b1)
{
    matrix view;
    matrix projection;
}
PixelShaderInput main(VertexShaderInput input)
{
    // ���ʿ��� ����鵵 VertexShaderInput�� ���Ͻ��ױ� ������ ä����� �մϴ�.
    
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f);

    pos = mul(pos, model); // Identity

    output.posWorld = pos.xyz;
    
    float4 normal = float4(input.normalModel, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);

    pos = mul(pos, view);
    
    pos = mul(pos, projection);
    output.posProj = pos;


    return output;
}
