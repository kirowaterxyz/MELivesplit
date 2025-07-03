#include "stdafx.h"

#include <chrono>
HRESULT(__stdcall *PresentOriginal)(IDirect3DDevice9 *pDevice, RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion);
std::chrono::steady_clock::time_point lastCaptureTime;
LPDIRECT3DTEXTURE9 liveSplitTexture = NULL;
HWND liveSplithWnd = NULL;

#define WATERMARK "megem is shit, jeeryy  bott"

void WriteText(LPDIRECT3DDEVICE9 device, int pt, UINT weight, DWORD align, char *font, DWORD color, int x, int y, char *text, int length) {
	LPD3DXFONT lpFont;
	D3DXCreateFontA(device, pt, 0, weight, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font, &lpFont);

	RECT r = { x, y, x, y };
	lpFont->DrawTextA(NULL, text, length, &r, DT_NOCLIP | align, color);
	lpFont->Release();
}

HBITMAP CaptureLiveSplitWindow(HWND hwnd) {
	HDC hdcWindow = GetDC(hwnd);
	HDC hdcMemDC = CreateCompatibleDC(hdcWindow);

	RECT rc;
	GetClientRect(hwnd, &rc);

	HBITMAP hbmScreen = CreateCompatibleBitmap(hdcWindow, rc.right - rc.left, rc.bottom - rc.top);
	SelectObject(hdcMemDC, hbmScreen);

	BitBlt(hdcMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcWindow, 0, 0, SRCCOPY);

	DeleteDC(hdcMemDC);
	ReleaseDC(hwnd, hdcWindow);

	return hbmScreen;
}

LPDIRECT3DTEXTURE9 CreateTextureFromHBITMAP(LPDIRECT3DDEVICE9 device, HBITMAP hBitmap) {
	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);

	LPDIRECT3DTEXTURE9 texture = NULL;
	HRESULT hr = D3DXCreateTexture(device, bm.bmWidth, bm.bmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
	if (FAILED(hr)) {
		return NULL;
	}

	D3DLOCKED_RECT rect;
	hr = texture->LockRect(0, &rect, NULL, 0);
	if (FAILED(hr)) {
		texture->Release();
		return NULL;
	}

	HDC hdcBitmap = CreateCompatibleDC(NULL);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcBitmap, hBitmap);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bm.bmWidth;
	bmi.bmiHeader.biHeight = -bm.bmHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	GetDIBits(hdcBitmap, hBitmap, 0, bm.bmHeight, rect.pBits, &bmi, DIB_RGB_COLORS);

	SelectObject(hdcBitmap, hOldBitmap);
	DeleteDC(hdcBitmap);

	texture->UnlockRect(0);
	return texture;
}

void UpdateLiveSplitTexture(LPDIRECT3DDEVICE9 device) {
	auto now = std::chrono::steady_clock::now();
	if ((now - lastCaptureTime).count()) {
		if (liveSplithWnd == NULL) {
			liveSplithWnd = FindWindowA(NULL, "LiveSplit");
		}
		if (liveSplithWnd) {
			HBITMAP hBitmap = CaptureLiveSplitWindow(liveSplithWnd);
			if (hBitmap) {
				if (liveSplitTexture) {
					liveSplitTexture->Release();
				}
				liveSplitTexture = CreateTextureFromHBITMAP(device, hBitmap);
				DeleteObject(hBitmap);
			}
		}
		lastCaptureTime = now;
	}
}

HRESULT __stdcall PresentHook(IDirect3DDevice9 *pDevice, RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) {
	UpdateLiveSplitTexture(pDevice);

	if (liveSplitTexture) {
		pDevice->SetTexture(0, liveSplitTexture);
		pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
		struct Vertex {
			float x, y, z, rhw;
			float u, v;
		} vertices[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
			{ 1940.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
			{ 0.0f, 44.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1940.0f, 44.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // tip // 1940 = width // 44 = height
		};
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));
		WriteText(pDevice, 25, FW_NORMAL, DT_LEFT, "Arial", D3DCOLOR_ARGB(255, 255, 0, 0), 5, 5, WATERMARK, strlen(WATERMARK));
	}

	return true ? PresentOriginal(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion) : D3D_OK;
}

void RenderD3D9() {

	//TrampolineHook(PresentHook, (void *)GetD3D9Exports()[D3D9_EXPORT_PRESENT], (void **)&PresentOriginal);
	GetD3D9Exports();
}
