#pragma once
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <d3d11.h>
#include <tchar.h>
#include <encryption/xorstr.hpp>
#include <encryption/lazy.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
namespace device
{
    namespace vars
    {
        ID3D11Device* g_pd3dDevice = nullptr;
        ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
        IDXGISwapChain* g_pSwapChain = nullptr;
        UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
        ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    }

    bool CreateDeviceD3D( HWND hWnd )
    {
        using namespace vars;
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory( &sd, sizeof( sd ) );
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext );
        if ( res == DXGI_ERROR_UNSUPPORTED ) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext );
        if ( res != S_OK )
            return false;

        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
        g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_mainRenderTargetView );
        pBackBuffer->Release( );
        return true;
    }

    void CleanupDeviceD3D( )
    {
        using namespace vars;
        if ( g_mainRenderTargetView ) { g_mainRenderTargetView->Release( ); g_mainRenderTargetView = nullptr; }
        if ( g_pSwapChain ) { g_pSwapChain->Release( ); g_pSwapChain = nullptr; }
        if ( g_pd3dDeviceContext ) { g_pd3dDeviceContext->Release( ); g_pd3dDeviceContext = nullptr; }
        if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = nullptr; }
    }

    void ResetDevice( )
    {
        using namespace vars;
    }

    void pre_render( )
    {
        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );
    }

    void post_render( )
    {
        using namespace vars;
        ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

        ImGui::Render( );
        const float clear_color_with_alpha[ 4 ] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets( 1, &g_mainRenderTargetView, nullptr );
        g_pd3dDeviceContext->ClearRenderTargetView( g_mainRenderTargetView, clear_color_with_alpha );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        g_pSwapChain->Present( 1, 0 );
    }

    LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        using namespace vars;
        if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
            return true;

        switch ( msg ) {
        case WM_SIZE:
            if ( wParam == SIZE_MINIMIZED )
                return 0;
            g_ResizeWidth = ( UINT )LOWORD( lParam ); // Queue resize
            g_ResizeHeight = ( UINT )HIWORD( lParam );
            return 0;
        case WM_SYSCOMMAND:
            if ( ( wParam & 0xfff0 ) == SC_KEYMENU ) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0;
        }
        return ::DefWindowProcW( hWnd, msg, wParam, lParam );
    }

    void shutdown( )
    {
        ImGui_ImplDX11_Shutdown( );
        ImGui_ImplWin32_Shutdown( );
        ImGui::DestroyContext( );

        CleanupDeviceD3D( );
    }
}
