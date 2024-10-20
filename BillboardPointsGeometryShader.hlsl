
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

struct GeometryShaderInput
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION; // Screen position
    float4 posWorld : POSITION0;
    float4 center : POSITION1;
    float2 texCoord : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GeometryShaderInput input[1], uint primID : SV_PrimitiveID,
                              inout TriangleStream<PixelShaderInput> outputStream)
{
    float hw = 0.5 * width;
    
    float4 up = float4(0.0, 1.0, 0.0, 0.0);
    float4 front = float4(eyeWorld, 1.0) - input[0].pos;
    //float4 front = float4(0.0f, 0.0f, -1.0f, 0.0);
    front.w = 0.0; // 벡터
    
    // 빌보드가 시점을 바라보는 방향 기준으로 오른쪽
    // 시점에서 빌보드를 바라보는 방향에서는 왼쪽 (텍스춰 좌표 주의)
    
    float4 right = float4(cross(up.xyz, normalize(front.xyz)), 0.0);
    
    //float4 right = float4(1.0, 0.0, 0.0, 0.0);
    PixelShaderInput output;
    
    output.center = input[0].pos; // 빌보드의 중심
    
    output.posWorld = input[0].pos - hw * right - hw * up;
    output.pos = output.posWorld;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.texCoord = float2(1.0, 1.0);

    
    outputStream.Append(output);

    output.posWorld = input[0].pos - hw * right + hw * up;
    output.pos = output.posWorld;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.texCoord = float2(1.0, 0.0);

    
    outputStream.Append(output);
    
    output.posWorld = input[0].pos + hw * right - hw * up;
    output.pos = output.posWorld;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.texCoord = float2(0.0, 1.0);

    
    outputStream.Append(output);
    
    output.posWorld = input[0].pos + hw * right + hw * up;
    output.pos = output.posWorld;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.texCoord = float2(0.0, 0.0);

    
    outputStream.Append(output);


}
