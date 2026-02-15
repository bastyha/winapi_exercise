#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	int nCmdShow
) {
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);	

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Learn",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
		
	if (hwnd == NULL)
	{
		return 0;
	}
	
	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);

	return 0;
	
}

void OnPaint(HDC hdc, PAINTSTRUCT ps) {
	FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));
	Graphics graphics(hdc);
	Bitmap bm(L"earth.png");
	int width = ps.rcPaint.right - ps.rcPaint.left;
	int heigth = ps.rcPaint.bottom - ps.rcPaint.top;
	int imageSide = min(width, heigth);	
	Rect destRect( width/2-imageSide/2, heigth/2-imageSide/2, imageSide , imageSide);
	graphics.DrawImage(
		&bm,
		destRect, 
		0, 0,
		bm.GetWidth(),
		bm.GetHeight(),
		UnitPixel
	);
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				OnPaint(hdc, ps);
				EndPaint(hwnd, &ps);
			}
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}