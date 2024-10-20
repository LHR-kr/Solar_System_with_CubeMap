#pragma once

#include <algorithm>
#include <iostream>
#include <memory>

#include "AppBase.h"
#include "GeometryGenerator.h"
#include "CubeMapMesh.h"
#include "Camera.h"

class RenderingApp : public AppBase {
  public:
    RenderingApp();

    virtual bool Initialize() override;
    virtual void UpdateGUI() override;
    virtual void Update(float dt) override;
    virtual void Render() override;

    virtual void OnMouseWheel(WPARAM wParam) override;
    virtual void OnMouseMove(LPARAM lParam) override;

  protected:

    // 하나의 3D 모델이 내부적으로는 여러개의 메쉬로 구성
    std::vector<shared_ptr<Mesh>> m_meshes;

;

    BasicPixelConstantBufferData m_pixelConstantBufferData;
    VertexViewProjConstantBufferData m_vertexViewProjConstantBufferData;
    BillboardConstantBufferData m_billBoardConstantBufferData;

    ComPtr<ID3D11Buffer> m_vertexViewProjConstantBuffer;
    ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

   
 
    int m_lightType = 0;
    Light m_lightFromGUI;
    float m_materialDiffuse = 1.0f;
    float m_materialSpecular = 1.0f;
    bool m_useWirefame = false;

    Camera m_camera;
};
