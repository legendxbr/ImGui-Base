#include <render/render.hpp>

int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
    #ifdef WINDOW_DEBUG
    {
        FILE* conout;
        AllocConsole( );
        SetConsoleTitleA( ( xorstr( "DEBUG" ) ) );
        freopen_s( &conout, ( xorstr( "conout$" ) ), xorstr( "w" ), stdout );
    }
    #endif

    if ( !others::utils::is_process_with_admin_rights( ) ) {
        LI_FN( MessageBoxA ).cached( )( 0, xorstr( "open it as administrator" ), others::string::random( 12 ).c_str( ), 0 );
        return EXIT_FAILURE;
    }

    if ( !window::create_window( xorstr( "ImGui Base" ), { 800,600 } ) ) {
        LI_FN( MessageBoxA ).cached( )( 0, xorstr( "unable to create window" ), others::string::random( 12 ).c_str( ), 0 );
        return EXIT_FAILURE;
    }

    if ( !window::create_device( ) ) {
        LI_FN( MessageBoxA ).cached( )( 0, xorstr( "unable to create device" ), others::string::random( 12 ).c_str( ), 0 );
        return EXIT_FAILURE;
    }

    if ( !render::init( ) ) {
        LI_FN( MessageBoxA ).cached( )( 0, xorstr( "unable to setup render" ), others::string::random( 12 ).c_str( ), 0 );
        return EXIT_FAILURE;
    }

    if ( !render::window( ) ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
