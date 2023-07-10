#include "imageloader.hpp"

// Lightweight CUtlBuffer
struct FileData_t
{
	FileData_t(char* pFileBuffer, int nFileSize)
	{
		m_Memory.m_pMemory = pFileBuffer;
		m_Memory.m_nAllocationCount = nFileSize;
		m_Memory.m_nGrowSize = 0;
		m_Get = 0;
		m_Put = nFileSize;
		m_Error = (char)0;
		m_Flags = (char)0;
		m_Reserved = (char)204;
		m_nTab = 0;
		m_nMaxPut = nFileSize;
		m_nOffset = 0;
	}

	struct MemData_t
	{
		char* m_pMemory;
		int m_nAllocationCount;
		int m_nGrowSize;
	} m_Memory;

	int m_Get;
	int m_Put;

	unsigned char m_Error;
	unsigned char m_Flags;
	unsigned char m_Reserved;

	int m_nTab;
	int m_nMaxPut;
	int m_nOffset;
};


//-----------------------------------------------------------------------------
// Tools
//-----------------------------------------------------------------------------

void ImageLoader::FreeImage(char* pImageBuffer)
{
	//I::MemAlloc->Free(pImageBuffer);
}

void ImageLoader::ConvertBGRtoRGB(char* pImageBuffer, int nSize)
{
	if (!pImageBuffer)
		return;

	for (int i = 0; i < nSize; i += 4)
	{
		BYTE temp = pImageBuffer[i + 0];
		pImageBuffer[i + 0] = pImageBuffer[i + 2];
		pImageBuffer[i + 2] = temp;
	}
}

void ImageLoader::ConvertBGRtoRGB(const unsigned char* pImageBuffer, int width, int height)
{
	if (!pImageBuffer)
		return;

	
}

//-----------------------------------------------------------------------------
// Create d3d9 texture by using clean image data
//-----------------------------------------------------------------------------

bool ImageLoader::CreateTexture(IDirect3DDevice9* pDevice, char* pImageBuffer, int iWidth, int iHeight, OUT IDirect3DTexture9** ppTexture)
{
	HRESULT hr = pDevice->CreateTexture(iWidth, iHeight, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ppTexture, 0);
	if (!*ppTexture)
		return false;

	D3DLOCKED_RECT Rect;
	IDirect3DTexture9* pTexture = *ppTexture;
	pTexture->LockRect(0, &Rect, 0, D3DLOCK_DISCARD);
	memcpy(Rect.pBits, pImageBuffer, iWidth * iHeight * 4);
	pTexture->UnlockRect(0);

	return true;
}

bool ImageLoader::CreateTexture(IDirect3DDevice9* pDevice, const unsigned char* pImageBuffer, int iWidth, int iHeight, OUT IDirect3DTexture9** ppTexture)
{
	HRESULT hr = pDevice->CreateTexture(iWidth, iHeight, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ppTexture, 0);
	if (!*ppTexture)
		return false;

	D3DLOCKED_RECT Rect;
	IDirect3DTexture9* pTexture = *ppTexture;
	pTexture->LockRect(0, &Rect, 0, D3DLOCK_DISCARD);
	memcpy(Rect.pBits, pImageBuffer, iWidth * iHeight * 4);

	for (int y = 0; y < iHeight; ++y) {
		const auto src = reinterpret_cast<const int*>(pImageBuffer + iWidth * 4 * y);
		const auto dest = reinterpret_cast<int*>((unsigned char*)Rect.pBits + Rect.Pitch * y);

		// RGBA --> BGRA
		for (int i = 0; i < iWidth; ++i) {
			auto color = src[i];
			color = (color & 0xFF00FF00) | ((color & 0xFF0000) >> 16) | ((color & 0xFF) << 16);
			dest[i] = color;
		}
	}

	pTexture->UnlockRect(0);
	return true;
}
