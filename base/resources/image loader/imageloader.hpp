#pragma once
#include <d3d9.h>

namespace ImageLoader
{
	// Tools
	void FreeImage(char* pImageBuffer);
	void ConvertBGRtoRGB(char* pImageBuffer, int nSize);
	void ConvertBGRtoRGB(const unsigned char* pImageBuffer, int width, int height);
 
	// Create d3d9 texture by using clean image data
	bool CreateTexture(IDirect3DDevice9* pDevice, char* pImageBuffer, int iWidth, int iHeight, OUT IDirect3DTexture9** ppTexture);
	bool CreateTexture(IDirect3DDevice9* pDevice, const unsigned char* pImageBuffer, int iWidth, int iHeight, OUT IDirect3DTexture9** ppTexture);
}
