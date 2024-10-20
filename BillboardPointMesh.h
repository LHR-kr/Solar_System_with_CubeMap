#pragma once
#include "Mesh.h"
class BillboardPointMesh : public Mesh 
{

  public:
    BillboardConstantBufferData m_billboardConstantBufferData;
    ComPtr<ID3D11Buffer> m_billboardConstantBuffer;
  public:
    virtual void Render(ComPtr<ID3D11DeviceContext> &context) override;
    virtual void Initialize(ComPtr<ID3D11Device> &device,
                            const GeometryMeshData &meshData,
                            const std::wstring VertexShaderFileName,
                            const std::wstring PixelShaderFileName,
                            const std::wstring HullShaderFileName = L"",
                            const std::wstring DomainShaderFileName = L"",
                            const std::wstring GeometryShaderFileName = L"") override;
};
