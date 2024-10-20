#include "BillboardPointMesh.h"
#include "D3D11Utils.h"

void BillboardPointMesh::Render(ComPtr<ID3D11DeviceContext> &context) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->VSSetShader(m_vertexShader.Get(), 0, 0);
    context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    context->PSSetShader(m_pixelShader.Get(), 0, 0);
    context->PSSetShaderResources(0, 1, m_textureResourceView.GetAddressOf());

    context->VSSetConstantBuffers(0, 1, m_billboardConstantBuffer.GetAddressOf());
    context->PSSetConstantBuffers(0, 1, m_billboardConstantBuffer.GetAddressOf());
    context->GSSetConstantBuffers(0, 1, m_billboardConstantBuffer.GetAddressOf());
    
    context->GSSetShader(m_geometryShader.Get(), 0, 0);


    context->IASetInputLayout(m_inputLayout.Get());
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride,
                                &offset);
 
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    context->Draw(m_indexCount, 0);

    context->GSSetShader(nullptr, 0, 0);
}

void BillboardPointMesh::Initialize(ComPtr<ID3D11Device> &device,
                                    const GeometryMeshData &meshData,
                                    const std::wstring VertexShaderFileName,
                                    const std::wstring PixelShaderFileName,
                                    const std::wstring HullShaderFileName,
                                    const std::wstring DomainShaderFileName,
                                    const std::wstring GeometryShaderFileName) {
    D3D11Utils::CreateSamplerState(device, m_samplerState);
    D3D11Utils::CreateConstantBuffer(device, m_billboardConstantBufferData,
                                     m_billboardConstantBuffer);
    

    D3D11Utils::CreateVertexBuffer(device, meshData.vertices, m_vertexBuffer);
    m_indexCount = UINT(meshData.indices.size());


    if (!meshData.textureFilename.empty()) {

        std::cout << meshData.textureFilename << std::endl;
        D3D11Utils::CreateTexture(device, meshData.textureFilename, m_texture2D,
                                  m_textureResourceView);
    }

    vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(device, VertexShaderFileName,
                                                 inputElements, m_vertexShader,
                                                 m_inputLayout);

    D3D11Utils::CreatePixelShader(device, PixelShaderFileName, m_pixelShader);

    !HullShaderFileName.empty()
        ? D3D11Utils::CreateHullShader(device, HullShaderFileName, m_hullShader)
        : false;
    !DomainShaderFileName.empty()
        ? D3D11Utils::CreateDomainShader(device, DomainShaderFileName,
                                         m_domainShader)
        : false;
    !GeometryShaderFileName.empty()
        ? D3D11Utils::CreateGeometryShader(device, GeometryShaderFileName,
                                           m_geometryShader)
        : false;

}
