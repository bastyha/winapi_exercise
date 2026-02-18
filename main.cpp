#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include "Application.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	int nCmdShow
) {
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Application app(hInstance);
	ShowWindow(app.getHwnd(), nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0;
}
