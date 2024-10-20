#pragma once

#include <directxtk\SimpleMath.h>

#include <vector>
#include <iostream>
#include <DirectXMath.h>
#include <d3d11.h>

#include <windows.h>
#include <wrl.h> // ComPtr
#include <vector>

#include "ConstanBuffertData.h"
#include "GeometryMeshData.h"

using Microsoft::WRL::ComPtr;
using std::vector;



class Mesh {


  public:
    Vector3 worldPosition;

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;

    VertexModelConstantBufferData m_vertexModelConstantData;
    VertexViewProjConstantBufferData m_vertexViewProjConstantData;
    BasicPixelConstantBufferData m_pixelConstantBufferData;

    ComPtr < ID3D11Buffer > m_vertexModelConstantBuffer;
    ComPtr<ID3D11Buffer> m_vertexViewProjConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11SamplerState> m_samplerState;
    ComPtr<ID3D11Texture2D> m_texture2D;
    ComPtr<ID3D11ShaderResourceView> m_textureResourceView;

    ComPtr<ID3D11HullShader> m_hullShader;
    ComPtr<ID3D11DomainShader> m_domainShader;
    ComPtr<ID3D11GeometryShader> m_geometryShader;

    UINT m_indexCount = 0;

    vector<Mesh *> childMeshes;
    
    virtual void CreateTexture(const std::string filename,
                               ComPtr<ID3D11Device> device);


    virtual void Render(ComPtr<ID3D11DeviceContext>& context);
    virtual void Initialize(ComPtr<ID3D11Device> &device,
                    const GeometryMeshData &meshData,
                    const std::wstring VertexShaderFileName,
                    const std::wstring PixelShaderFileName,
                    const std::wstring HullShaderFileName = L"",
                    const std::wstring DomainShaderFileName = L"",
                    const std::wstring GeometryShaderFileName = L"");


};
