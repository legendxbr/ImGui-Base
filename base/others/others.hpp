#pragma once
#include <common.h>
#include <implementations/utf8/utf8.h>
#include <random>

namespace others
{
	namespace string
	{
		std::string random( int i_size )
		{
			static auto& chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

			thread_local static std::mt19937 rg{ std::random_device{}( ) };
			thread_local static std::uniform_int_distribution<std::string::size_type> pick( 0, sizeof( chars ) - 2 );

			std::string string;
			string.reserve( i_size );

			while ( i_size-- )
				string += chars[ pick( rg ) ];

			return string;
		}

        std::string to_lower( std::string str )
        {
            transform( str.begin( ), str.end( ), str.begin( ), static_cast< int( * )( int ) >( ::tolower ) );
            return str;
        }

        std::string sanitize_utf8( std::string in )
        {
            std::string temp;
            utf8::replace_invalid( in.begin( ), in.end( ), back_inserter( temp ) );
            return temp;
        }
	}

    namespace utils
    {
        bool is_process_with_admin_rights( )
        {
            auto h_handle = LI_FN( GetCurrentProcess ).cached( )( );

            auto f_ret = false;
            HANDLE h_token = nullptr;

            if ( OpenProcessToken( h_handle, TOKEN_QUERY, &h_token ) ) {
                TOKEN_ELEVATION elevation;
                unsigned long cb_size = sizeof( TOKEN_ELEVATION );
                if ( LI_FN( GetTokenInformation ).cached( )( h_token, TokenElevation, &elevation, sizeof( elevation ), &cb_size ) )
                    f_ret = elevation.TokenIsElevated;
            }

            if ( h_token )
                LI_FN( CloseHandle ).cached( )( h_token );

            if ( h_handle )
                LI_FN( CloseHandle ).cached( )( h_handle );

            return f_ret;
        }
    }
}