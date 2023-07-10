#pragma once
#include <imgui/imgui_internal.h>
#include <device/device.hpp>
#include <iostream>
#include <string>
#include <dwmapi.h>
#include <vector>
#include <thread>
#include <algorithm>

namespace window
{
    namespace vars
    {
        HWND hwnd;
        WNDCLASSEXA wc;
    }

    void set_window_in_center( HWND hwnd )
    {
        RECT rc;

        LI_FN( GetWindowRect ).cached( )( hwnd, &rc );

        int xPos = ( GetSystemMetrics( SM_CXSCREEN ) - rc.right ) / 2;
        int yPos = ( GetSystemMetrics( SM_CYSCREEN ) - rc.bottom ) / 2;

        LI_FN( SetWindowPos ).cached( )( hwnd, nullptr, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    bool create_window( std::string windowname, std::pair<int, int> size)
    {
        using namespace vars;
        vars::wc = { sizeof( vars::wc ), 0x0040, device::WndProc, 0L, 0L, LI_FN( GetModuleHandleA ).cached( )( nullptr ), nullptr, nullptr, nullptr, nullptr, windowname.c_str( ), nullptr };
        LI_FN( RegisterClassExA ).cached( )( &vars::wc );
        vars::hwnd = LI_FN( CreateWindowExA ).cached( )( 0, vars::wc.lpszClassName, windowname.c_str( ), WS_POPUP, 100, 100, size.first, size.second, nullptr, nullptr, vars::wc.hInstance, nullptr );

        if ( !vars::hwnd )
            return false;

        LI_FN( SetWindowRgn ).cached( )( vars::hwnd, LI_FN( CreateRoundRectRgn ).cached( )( 0, 0, size.first, size.second, 15, 15 ), true );

        set_window_in_center( vars::hwnd );
        return true;
    }

    bool create_device( )
    {
        if ( !device::CreateDeviceD3D( vars::hwnd ) ) {
            device::CleanupDeviceD3D( );
            LI_FN( UnregisterClassA ).cached( )( vars::wc.lpszClassName, vars::wc.hInstance );
            return false;
        }

        LI_FN( ShowWindow ).cached( )( vars::hwnd, SW_SHOWDEFAULT );
        LI_FN( UpdateWindow ).cached( )( vars::hwnd );
        return true;
    }

    void destroy_window( )
    {
        LI_FN( DestroyWindow ).cached( )( vars::hwnd );
        LI_FN( UnregisterClassA ).cached( )( vars::wc.lpszClassName, vars::wc.hInstance );
    }
}
