#include "CubeMapMesh.h"
#include <directxtk/DDSTextureLoader.h> // Å¥ºê¸Ê ÀÐÀ» ¶§ ÇÊ¿ä



using namespace std;
using namespace DirectX;

void CubeMapMesh::CreateTexture(const string filename,
                                ComPtr<ID3D11Device> device) {
    // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
    wstring wfileName = wstring(filename.begin(), filename.end());

    auto hr = CreateDDSTextureFromFileEx(
        device.Get(), wfileName.c_str(), 0, D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE, 0,
        D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
        DDS_LOADER_FLAGS(false), (ID3D11Resource **)m_texture2D.GetAddressOf(),
        m_textureResourceView.GetAddressOf(), nullptr);

    if (FAILED(hr)) {
        std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
    }
}
