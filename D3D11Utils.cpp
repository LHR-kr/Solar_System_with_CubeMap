#define _CRT_SECURE_NO_WARNINGS // stb_image_write compile error fix

#include "D3D11Utils.h"

#include <directxtk/DDSTextureLoader.h> // 큐브맵 읽을 때 필요
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"



using namespace std;
using namespace DirectX;

void CheckResult(HRESULT hr, ID3DBlob *errorBlob) {
    if (FAILED(hr)) {
        // 파일이 없을 경우
        if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
            cout << "File not found." << endl;
        }

        // 에러 메시지가 있으면 출력
        if (errorBlob) {
            cout << "Shader compile error\n"
                 << (char *)errorBlob->GetBufferPointer() << endl;
        }
    }
}

bool D3D11Utils::CreateDepthBuffer(
    ComPtr<ID3D11Device> &device, int screenWidth, int screenHeight,
    UINT &numQualityLevels, ComPtr<ID3D11DepthStencilView> &depthStencilView) {

    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    depthStencilBufferDesc.Width = screenWidth;
    depthStencilBufferDesc.Height = screenHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (numQualityLevels > 0) {
        depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
    } else {
        depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = 0;
    }
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.MiscFlags = 0;

    ComPtr<ID3D11Texture2D> depthStencilBuffer;

    if (FAILED(device->CreateTexture2D(&depthStencilBufferDesc, 0,
                                       depthStencilBuffer.GetAddressOf()))) {
        std::cout << "CreateTexture2D() failed." << std::endl;
    }
    if (FAILED(device->CreateDepthStencilView(
            depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf()))) {
        std::cout << "CreateDepthStencilView() failed." << std::endl;
    }
    return true;
}

void D3D11Utils::CreateVertexShaderAndInputLayout(
    ComPtr<ID3D11Device> &device, const wstring &filename,
    const vector<D3D11_INPUT_ELEMENT_DESC> &inputElements,
    ComPtr<ID3D11VertexShader> &m_vertexShader,
    ComPtr<ID3D11InputLayout> &m_inputLayout) {

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    CheckResult(hr, errorBlob.Get());

    device->CreateVertexShader(shaderBlob->GetBufferPointer(),
                               shaderBlob->GetBufferSize(), NULL,
                               &m_vertexShader);

    device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
                              shaderBlob->GetBufferPointer(),
                              shaderBlob->GetBufferSize(), &m_inputLayout);
}

void D3D11Utils::CreateHullShader(ComPtr<ID3D11Device> &device,
                                  const wstring &filename,
                                  ComPtr<ID3D11HullShader> &m_hullShader) {
    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "hs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    CheckResult(hr, errorBlob.Get());

    device->CreateHullShader(shaderBlob->GetBufferPointer(),
                             shaderBlob->GetBufferSize(), NULL, &m_hullShader);
}

void D3D11Utils::CreateDomainShader(
    ComPtr<ID3D11Device> &device, const wstring &filename,
    ComPtr<ID3D11DomainShader> &m_domainShader) {

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "ds_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    CheckResult(hr, errorBlob.Get());

    device->CreateDomainShader(shaderBlob->GetBufferPointer(),
                               shaderBlob->GetBufferSize(), NULL,
                               &m_domainShader);
}

void D3D11Utils::CreatePixelShader(ComPtr<ID3D11Device> &device,
                                   const wstring &filename,
                                   ComPtr<ID3D11PixelShader> &m_pixelShader) {
    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    CheckResult(hr, errorBlob.Get());

    device->CreatePixelShader(shaderBlob->GetBufferPointer(),
                              shaderBlob->GetBufferSize(), NULL,
                              &m_pixelShader);
}

void D3D11Utils::CreateIndexBuffer(ComPtr<ID3D11Device> &device,
                                   const std::vector<uint32_t> &indices,
                                   ComPtr<ID3D11Buffer> &indexBuffer) {
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
    bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
    bufferDesc.StructureByteStride = sizeof(uint32_t);

    D3D11_SUBRESOURCE_DATA indexBufferData = {0};
    indexBufferData.pSysMem = indices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    device->CreateBuffer(&bufferDesc, &indexBufferData,
                         indexBuffer.GetAddressOf());
}

void D3D11Utils::CreateGeometryShader(
    ComPtr<ID3D11Device> &device, const wstring &filename,
    ComPtr<ID3D11GeometryShader> &geometryShader) {

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "gs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    // CheckResult(hr, errorBlob.Get());

    device->CreateGeometryShader(shaderBlob->GetBufferPointer(),
                                 shaderBlob->GetBufferSize(), NULL,
                                 &geometryShader);
}

void ReadImage(const std::string filename, std::vector<uint8_t> &image,
               int &width, int &height) {

    int channels;

    unsigned char *img =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // assert(channels == 4);

    // 4채널로 만들어서 복사
    image.resize(width * height * 4);

    if (channels == 3) {
        for (size_t i = 0; i < width * height; i++) {
            for (size_t c = 0; c < 3; c++) {
                image[4 * i + c] = img[i * channels + c];
            }
            image[4 * i + 3] = 255;
        }
    } else if (channels == 4) {
        for (size_t i = 0; i < width * height; i++) {
            for (size_t c = 0; c < 4; c++) {
                image[4 * i + c] = img[i * channels + c];
            }
        }
    } else {
        std::cout << "Read 3 or 4 channels images only. " << channels
                  << " channels" << endl;
    }
}

void D3D11Utils::CreateTexture(
    ComPtr<ID3D11Device> &device, const std::string filename,
    ComPtr<ID3D11Texture2D> &texture,
    ComPtr<ID3D11ShaderResourceView> &textureResourceView) {

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

    device->CreateTexture2D(&txtDesc, &initData, texture.GetAddressOf());
    device->CreateShaderResourceView(texture.Get(), nullptr,
                                     textureResourceView.GetAddressOf());
}

void D3D11Utils::CreateTextureArray(
    ComPtr<ID3D11Device> &device, const std::vector<std::string> filenames,
    ComPtr<ID3D11Texture2D> &texture,
    ComPtr<ID3D11ShaderResourceView> &textureResourceView) {

    if (filenames.empty())
        return;

    // 모든 이미지의 width와 height가 같다고 가정합니다.

    int width = 0, height = 0;
    std::vector<uint8_t> imageArray;
    for (const auto &f : filenames) {

        cout << f << endl;

        std::vector<uint8_t> image;

        ReadImage(f, image, width, height);

        imageArray.insert(imageArray.begin(), image.begin(), image.end());
    }

    // Create texture.
    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = UINT(width);
    txtDesc.Height = UINT(height);
    txtDesc.MipLevels = 1;
    txtDesc.ArraySize = UINT(filenames.size());
    txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.SampleDesc.Quality = 0;
    txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // SUBRESOURCE_DATA의 배열
    std::vector<D3D11_SUBRESOURCE_DATA> initData(filenames.size());
    size_t offset = 0;
    for (auto &i : initData) {
        i.pSysMem = imageArray.data() + offset;
        i.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
        i.SysMemSlicePitch =
            txtDesc.Width * txtDesc.Height * sizeof(uint8_t) * 4;
        offset += i.SysMemSlicePitch;
    }

    device->CreateTexture2D(&txtDesc, initData.data(), texture.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Format = txtDesc.Format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    desc.Texture2DArray.MostDetailedMip = 0;
    desc.Texture2DArray.MipLevels = txtDesc.MipLevels;
    desc.Texture2DArray.FirstArraySlice = 0;
    desc.Texture2DArray.ArraySize = txtDesc.ArraySize;

    device->CreateShaderResourceView(texture.Get(), &desc,
                                     textureResourceView.GetAddressOf());
}

void D3D11Utils::CreateCubemapTexture(
    ComPtr<ID3D11Device> &device, const wchar_t *filename,
    ComPtr<ID3D11ShaderResourceView> &textureResourceView) {

    ComPtr<ID3D11Texture2D> texture;

    // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
    auto hr = CreateDDSTextureFromFileEx(
        device.Get(), filename, 0, D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE, 0,
        D3D11_RESOURCE_MISC_TEXTURECUBE, // 큐브맵용 텍스춰
        DDS_LOADER_FLAGS(false), (ID3D11Resource **)texture.GetAddressOf(),
        textureResourceView.GetAddressOf(), nullptr);

    if (FAILED(hr)) {
        std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
    }
}

void D3D11Utils::WriteToFile(ComPtr<ID3D11Device> &device,
                             ComPtr<ID3D11DeviceContext> &context,
                             ComPtr<ID3D11Texture2D> &textureToWrite,
                             const std::string filename) {

    D3D11_TEXTURE2D_DESC desc;
    textureToWrite->GetDesc(&desc);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU에서 읽기 가능
    desc.Usage = D3D11_USAGE_STAGING; // GPU에서 CPU로 보낼 데이터를 임시 보관

    ComPtr<ID3D11Texture2D> stagingTexture;
    if (FAILED(device->CreateTexture2D(&desc, nullptr,
                                       stagingTexture.GetAddressOf()))) {
        cout << "Failed()" << endl;
    }

    // 참고: 전체 복사할 때
    // context->CopyResource(stagingTexture.Get(), pTemp.Get());

    // 일부만 복사할 때 사용
    D3D11_BOX box;
    box.left = 0;
    box.right = desc.Width;
    box.top = 0;
    box.bottom = desc.Height;
    box.front = 0;
    box.back = 1;
    context->CopySubresourceRegion(stagingTexture.Get(), 0, 0, 0, 0,
                                   textureToWrite.Get(), 0, &box);

    // R8G8B8A8 이라고 가정
    std::vector<uint8_t> pixels(desc.Width * desc.Height * 4);

    D3D11_MAPPED_SUBRESOURCE ms;
    context->Map(stagingTexture.Get(), NULL, D3D11_MAP_READ, NULL,
                 &ms); // D3D11_MAP_READ 주의

    // 텍스춰가 작을 경우에는
    // ms.RowPitch가 width * sizeof(uint8_t) * 4보다 클 수도 있어서
    // for문으로 가로줄 하나씩 복사
    uint8_t *pData = (uint8_t *)ms.pData;
    for (unsigned int h = 0; h < desc.Height; h++) {
        memcpy(&pixels[h * desc.Width * 4], &pData[h * ms.RowPitch],
               desc.Width * sizeof(uint8_t) * 4);
    }

    context->Unmap(stagingTexture.Get(), NULL);

    stbi_write_png(filename.c_str(), desc.Width, desc.Height, 4, pixels.data(),
                   desc.Width * 4);

    cout << filename << endl;
}

void D3D11Utils::CreateSamplerState(ComPtr<ID3D11Device> &device,
                                    ComPtr<ID3D11SamplerState> &samplerState) {
    // Sampler 만들기
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());
}



