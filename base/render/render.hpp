#pragma once
#include <common.h>

namespace render
{
    void style( )
    {
        ImGuiStyle* style = &ImGui::GetStyle( );
    }

    bool init( )
    {
        ImGui::CreateContext( );
        ImGuiIO& io = ImGui::GetIO( );

        ImGui::StyleColorsDark( );

        style( );

        if ( !ImGui_ImplWin32_Init( window::vars::hwnd ) )
            return false;

        return ImGui_ImplDX11_Init( device::vars::g_pd3dDevice, device::vars::g_pd3dDeviceContext );
    }

    void render( )
    {
        ImGuiIO& io = ImGui::GetIO( );
        ImGuiStyle* style = &ImGui::GetStyle( );
        auto i_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
        ImGui::SetNextWindowSize( { 200, 200 }, ImGuiCond_::ImGuiCond_Always );
        ImGui::Begin( xorstr( "imgui base" ), nullptr,  i_flags );
        {
            ImDrawList* drawlist = ImGui::GetWindowDrawList( );
            ImVec2 pos = ImGui::GetWindowPos( );
            ImVec2 size = ImGui::GetWindowSize( );
            ImGuiWindow* window = ImGui::GetCurrentWindow( );
        }
        ImGui::End( );
    }

    bool window( )
    {
        MSG msg;
        std::memset( &msg, 0, sizeof( msg ) );

        while ( msg.message != WM_QUIT ) {
            if ( LI_FN( PeekMessageA ).cached( )( &msg, nullptr, 0U, 0U, PM_REMOVE ) ) {
                LI_FN( TranslateMessage ).cached( )( &msg );
                LI_FN( DispatchMessageA ).cached( )( &msg );
                continue;
            }

            device::pre_render( );
            render( );
            device::post_render( );     
        }

        device::shutdown( );
        window::destroy_window( );
        return true;
    }
}
