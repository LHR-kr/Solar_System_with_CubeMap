#include "Mesh.h"
#include "stb_image.h"
#include "Vertex.h"
#include "D3D11Utils.h"

void Mesh::Initialize(ComPtr<ID3D11Device> &device,
                      const GeometryMeshData &meshData,
                      const std::wstring VertexShaderFileName,
                      const std::wstring PixelShaderFileName,
                      const std::wstring HullShaderFileName ,
                      const std::wstring DomainShaderFileName,
                      const std::wstring GeometryShaderFileName ) {
  
    D3D11Utils::CreateSamplerState(device, m_samplerState);
    // ConstantBuffer 만들기
    m_vertexModelConstantData.model = Matrix();
    m_vertexViewProjConstantData.view = Matrix();
    m_vertexViewProjConstantData.projection = Matrix();

    D3D11Utils::CreateConstantBuffer(device, m_vertexModelConstantData,
                                     m_vertexModelConstantBuffer);
    D3D11Utils::CreateConstantBuffer(device, m_vertexViewProjConstantData,
                                     m_vertexViewProjConstantBuffer);
    D3D11Utils::CreateConstantBuffer(device, m_pixelConstantBufferData,
                                     m_pixelConstantBuffer);

    D3D11Utils::CreateVertexBuffer(device, meshData.vertices, m_vertexBuffer);
    m_indexCount = UINT(meshData.indices.size());
    D3D11Utils::CreateIndexBuffer(device, meshData.indices, m_indexBuffer);

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




void Mesh::CreateTexture(const std::string filename,
                         ComPtr < ID3D11Device> device) {

    int width, height, channels;

    unsigned char *img =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // assert(channels == 4);

    // 4채널로 만들어서 복사
    std::vector<uint8_t> image;
    image.resize(width * height * 4);
    for (size_t i = 0; i < width * height; i++) {
        for (size_t c = 0; c < 3; c++) {
            image[4 * i + c] = img[i * channels + c];
        }
        image[4 * i + 3] = 255;
    }

    // Create texture.
    D3D11_TEXTURE2D_DESC txtDesc = {};
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = image.data();
    initData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
    // initData.SysMemSlicePitch = 0;

    device->CreateTexture2D(&txtDesc, &initData, m_texture2D.GetAddressOf());
    device->CreateShaderResourceView(m_texture2D.Get(), nullptr,
                                     m_textureResourceView.GetAddressOf());
}

void Mesh::Render(ComPtr<ID3D11DeviceContext> &context) 
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->VSSetShader(m_vertexShader.Get(), 0, 0);
    context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    context->PSSetShader(m_pixelShader.Get(), 0, 0);
    ID3D11Buffer *constants[] = {m_vertexModelConstantBuffer.Get(),
                                 m_vertexViewProjConstantBuffer.Get()};
    context->VSSetConstantBuffers(0, 2, constants);


 
    context->PSSetShaderResources(0, 1, m_textureResourceView.GetAddressOf());

    context->PSSetConstantBuffers(0, 1, m_pixelConstantBuffer.GetAddressOf());

    context->IASetInputLayout(m_inputLayout.Get());
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(),
                                &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                                0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_indexCount, 0, 0);

    
}

