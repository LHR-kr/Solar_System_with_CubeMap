#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <wrl.h> // ComPtr


using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using std::vector;
using std::wstring;

class AppBase {
  public:
    AppBase();
    virtual ~AppBase();

    float GetAspectRatio() const;

    int Run();

    virtual bool Initialize();
    virtual void UpdateGUI() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Convenience overrides for handling mouse input.
    virtual void OnMouseWheel(WPARAM wParam) = 0;
    virtual void OnMouseMove(LPARAM lParam) = 0;

    int m_screenWidth;
    int m_screenHeight;
    int m_guiWidth = 0;
    HWND m_mainWindow;
    UINT numQualityLevels = 0;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<IDXGISwapChain> m_swapChain;

    ComPtr<ID3D11RasterizerState> m_solidRasterizerSate;
    ComPtr<ID3D11RasterizerState> m_wireRasterizerSate;

    // Depth buffer 관련
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    D3D11_VIEWPORT m_screenViewport;

  protected: // 상속 받은 클래스에서도 접근 가능
    bool InitMainWindow();
    bool InitDirect3D();
    bool InitGUI();

    void SetViewport();
    bool CreateRenderTargetView();
 
     bool m_keyPressed[256] = {
        false,
    };

};
