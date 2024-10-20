#pragma once
#include "RenderingApp.h"
#include <tuple>
#include <vector>
#include <ctime>
#include "GeometryGenerator.h"
#include <windowsx.h>
#include "D3D11Utils.h"
#include "BillboardPointMesh.h"

using namespace std;
using namespace DirectX;

float radius[] = { 0.4f, 0.9f, 1.0f, 0.6f, 3.0f, 2.0f, 1.5f, 1.5f};
float revolutionSpeed[] = {1.6f, 1.3f, 1.0f, 0.97f, 0.8f, 0.7f, 0.6f, 0.55f, 0.7f,0.0f}; // 각속도(radian)
float rotationSpeed[] = { 1.5f, 1.2f, 1.0f, 1.1f, 0.5f, 0.7f, 0.8f, 0.78f, 0.7f, 0.0f};
float revolutionRadius[] = { 1.4f, 2.4f, 3.4f, 5.4f, 7.0f, 9.0f, 11.0f, 13.0f, 9.0f, 0.0f};

RenderingApp::RenderingApp() : AppBase() {}

void RenderingApp::OnMouseWheel(WPARAM wParam) {
    //// 최상단의 1 WORD만큼의 바이트를 가져와서 해석한다.
    //if ((short)HIWORD(wParam) > 0) {

    //    m_camera. = max(0.0f, m_projFovAngleY - 1);
    //} 
    //else
    //    m_projFovAngleY = min(180.0f, m_projFovAngleY + 1);
}

void RenderingApp::OnMouseMove(LPARAM lParam) {

    float delta = 0.08f;

    int newxPos = LOWORD(lParam);
    int newyPos = HIWORD(lParam);

    float cursorNdcX = newxPos * 2.0f / m_screenWidth - 1.0f;
    float cursorNdcY = -newyPos * 2.0f / m_screenHeight + 1.0f;


    // 커서가 화면 밖으로 나갔을 경우 범위 조절
    // 게임에서는 클램프를 안할 수도 있습니다.
    cursorNdcX = std::clamp(cursorNdcX, -1.0f, 1.0f);
    cursorNdcY = std::clamp(cursorNdcY, -1.0f, 1.0f);


    m_camera.UpdateMouse(cursorNdcX, cursorNdcY);

}

bool RenderingApp::Initialize() {

    if (!AppBase::Initialize())
        return false;

    {
        // 행성 Geometry 정의
        vector<GeometryMeshData> meshDatas;
        for (int i = 0; i < 8; i++) {
            meshDatas.push_back(
                GeometryGenerator::MakeSphere(radius[i] * 0.07, 50, 50));
        }
        // 토성 고리
        meshDatas.push_back(GeometryGenerator::MakeRing(0.2f, 0.25f, 40));

        for (const auto &meshData : meshDatas) {
            auto newMesh = std::make_shared<Mesh>();
            newMesh->Initialize(m_device, meshData, L"BasicVertexShader.hlsl",
                                L"BasicPixelShader.hlsl");
            this->m_meshes.push_back(newMesh);
        }

        D3D11Utils::CreateConstantBuffer(m_device,
                                         m_vertexViewProjConstantBufferData,
                                         m_vertexViewProjConstantBuffer);
        D3D11Utils::CreateConstantBuffer(m_device, m_pixelConstantBufferData,
                                         m_pixelConstantBuffer);

        // share buffer between meshes
        for (auto mesh : m_meshes) {
            mesh->m_vertexViewProjConstantBuffer =
                this->m_vertexViewProjConstantBuffer;
            mesh->m_pixelConstantBuffer = this->m_pixelConstantBuffer;
        }
    }

    

    // cube map
    {
        
        GeometryMeshData cubeMapMeshData = GeometryGenerator::MakeBox(20.f);

        reverse(cubeMapMeshData.indices.begin(), cubeMapMeshData.indices.end());
        shared_ptr<Mesh> newMesh = make_shared<CubeMapMesh>();

        newMesh->Initialize(m_device, cubeMapMeshData, L"BasicVertexShader.hlsl",
                            L"CubeMappingPixelShader.hlsl");

        newMesh->m_vertexViewProjConstantBuffer =
            this->m_vertexViewProjConstantBuffer;

        m_meshes.push_back(newMesh);
    }
   


    // 큐브맵  + 수금지화목토천해 텍스쳐
    vector<std::string> solarSystemTextureFileName = {
        "./Textures/mercury.jpg",     "./Textures/venus.jpg",
        "./Textures/earth.jpg",       "./Textures/mars.jpg",
        "./Textures/jupiter.jpg",     "./Textures/saturn.jpg",
        "./Textures/uranus.jpg",      "./Textures/neptune.jpg",
        "./Textures/saturn_ring.png", "./CubemapTextures/night.dds"};
    for (int i = 0; i < solarSystemTextureFileName.size(); i++)
    {
        m_meshes[i]->CreateTexture(solarSystemTextureFileName[i], m_device);
    }
     // billboard Sun
    { 
        auto point = GeometryGenerator::MakePoint();

        shared_ptr<Mesh> billPoint = make_shared<BillboardPointMesh>();
        billPoint->Initialize(m_device, point,
                              L"BillboardPointsVertexShader.hlsl",
                              L"SunPixelShader.hlsl", L"", L"",
                              L"BillboardPointsGeometryShader.hlsl");
        billPoint->CreateTexture("./Textures/sun_billboard.jpg", m_device);
        
        m_meshes.push_back(billPoint);
    }
    
    return true;
}

void RenderingApp::Update(float dt) {

    using namespace DirectX;
    static double acctime = 0.0;

    acctime += dt;

    if (m_keyPressed[87])
        m_camera.MoveForward(dt);
    if (m_keyPressed[83])
        m_camera.MoveForward(-dt);
    if (m_keyPressed[68])
        m_camera.MoveRight(dt);
    if (m_keyPressed[65])
        m_camera.MoveRight(-dt);


    // update each mesh's model constant buffer
    for (int i = 0; i < m_meshes.size()-1; i++) {
        Matrix modelMat = Matrix();
        Matrix invTransMat = Matrix();

        modelMat = Matrix::CreateRotationY(rotationSpeed[i] * acctime) *
                   Matrix::CreateTranslation(
                       Vector3(revolutionRadius[i] * 0.2, 0.0f, 0.0f)) *
                   Matrix::CreateRotationY(acctime * revolutionSpeed[i]);
        modelMat = modelMat.Transpose();

        invTransMat = modelMat;
        invTransMat.Translation(Vector3(0.0f));
        invTransMat = invTransMat.Transpose().Invert();

        m_meshes[i]->m_vertexModelConstantData.model = modelMat;
        m_meshes[i]->m_vertexModelConstantData.invTranspose = invTransMat;
        D3D11Utils::UpdateBuffer(m_device, m_context,
                                 m_meshes[i]->m_vertexModelConstantData,
                              m_meshes[i]->m_vertexModelConstantBuffer);
    }

    // view, proj 변환
    Matrix viewRow = m_camera.GetViewRow();
    Matrix projRow = m_camera.GetProjRow();
    Vector3 eyeWorld = m_camera.GetEyePos();
    
    // planet mesh + cube map mesh
    {
        m_vertexViewProjConstantBufferData.view = viewRow.Transpose();
        m_vertexViewProjConstantBufferData.projection = projRow.Transpose();

        D3D11Utils::UpdateBuffer(m_device, m_context,
                                 m_vertexViewProjConstantBufferData,
                                 m_vertexViewProjConstantBuffer);
    }
    
 


    //sun constant buffer update
    {
        BillboardConstantBufferData constData;
        constData.width = 0.6f;
        constData.time = acctime;
        constData.model = Matrix();
        constData.eyeWorld = eyeWorld;
         

        // view
        constData.view = viewRow.Transpose();
        // 프로젝션
        constData.proj = projRow.Transpose();
        BillboardPointMesh *billPoint =
            (BillboardPointMesh *)m_meshes.back().get();
        D3D11Utils::UpdateBuffer(m_device, m_context, constData,
                                 billPoint->m_billboardConstantBuffer);
    }


    m_pixelConstantBufferData.lights = m_lightFromGUI;
    m_pixelConstantBufferData.material.diffuse = Vector3(m_materialDiffuse);
    m_pixelConstantBufferData.material.specular = Vector3(m_materialSpecular);
    m_pixelConstantBufferData.eyeWorld = eyeWorld;
    D3D11Utils::UpdateBuffer(m_device, m_context, m_pixelConstantBufferData,
                             m_pixelConstantBuffer);
}

void RenderingApp::Render() {


    // RS: Rasterizer stage
    // OM: Output-Merger stage
    // VS: Vertex Shader
    // PS: Pixel Shader
    // IA: Input-Assembler stage

    SetViewport();

    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
                                     D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                     1.0f, 0);
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(),
                                  m_depthStencilView.Get());
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
    if (m_useWirefame)
        m_context->RSSetState(m_wireRasterizerSate.Get());
    else
        m_context->RSSetState(m_solidRasterizerSate.Get());

    for (auto& mesh : m_meshes)
    {
        mesh->Render(m_context);
    }
}

void RenderingApp::UpdateGUI() {

    ImGui::Checkbox("Wireframe", &m_useWirefame);
}


void AppBase::SetViewport() {

    static int previousGuiWidth = -1;

    if (previousGuiWidth != m_guiWidth) {

        previousGuiWidth = m_guiWidth;

        // Set the viewport
        ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
        m_screenViewport.TopLeftX = float(m_guiWidth);
        m_screenViewport.TopLeftY = 0;
        m_screenViewport.Width = float(m_screenWidth - m_guiWidth);
        m_screenViewport.Height = float(m_screenHeight);
        // m_screenViewport.Width = static_cast<float>(m_screenHeight);
        m_screenViewport.MinDepth = 0.0f;
        m_screenViewport.MaxDepth = 1.0f;

        m_context->RSSetViewports(1, &m_screenViewport);
    }
}

bool AppBase::CreateRenderTargetView() {

    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    if (backBuffer) {
        m_device->CreateRenderTargetView(backBuffer.Get(), NULL,
                                         m_renderTargetView.GetAddressOf());
    } else {
        std::cout << "CreateRenderTargetView() failed." << std::endl;
        return false;
    }

    return true;
}

