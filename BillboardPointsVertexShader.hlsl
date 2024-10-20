cbuffer BillboardPointsConstantData : register(b0)
{
    float3 eyeWorld;
    float width;
    Matrix model; // For vertex shader
    Matrix view; // For vertex shader
    Matrix proj; // For vertex shader
    float time;
    float3 padding;
};

struct VertexShaderInput
{
    float3 pos : POSITION; // �� ��ǥ���� ��ġ position
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct GeometryShaderInput
{
    float4 pos : SV_POSITION; // Screen position
    float4 normal : NORMAL;
};

GeometryShaderInput main(VertexShaderInput input)
{
    GeometryShaderInput output;
    
    // Geometry shader�� �״�� �Ѱ��ݴϴ�.
    output.pos = float4(input.pos, 1.0);
    output.normal = float4(input.normal, 0.0);
    
    return output;
}