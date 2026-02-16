#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <winuser.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#include <cmath>

#include "ApplicationState.cpp"


using namespace Gdiplus;

const int ANIMATION_TIMER_ID = 1;


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

	ApplicationState *appState = new ApplicationState();

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Learn",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		appState
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

double degToRad(double deg) {
	double pi = 3.14159265359;
	return deg * (pi / 180);
}

void rotateRectangleAroundCenter(double radius,double degree,  Point (&points)[3]) {
	double sinCoord = std::sin(degToRad(degree))*radius;
	double cosCoord = std::cos(degToRad(degree))*radius;
	points[0] = Point(-1 * sinCoord, cosCoord);
	points[1] = Point(cosCoord, sinCoord);
	points[2] = Point(sinCoord, -1 * cosCoord);
}

void pushImage(int amount,  Point(&points)[3]) {
	points[0] = points[0] - Point(amount, 0);
	points[1] = points[1] - Point(amount, 0);
	points[2] = points[2] - Point(amount, 0);
}

void onPaint(HDC hdc, PAINTSTRUCT ps, ApplicationState* appState) {
	FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));
	Graphics graphics(hdc);
	Bitmap bm(L"earth.png");
	int width = ps.rcPaint.right - ps.rcPaint.left;
	int heigth = ps.rcPaint.bottom - ps.rcPaint.top;
	int imageSide = min(width, heigth);	
	int x = width / 2 - imageSide / 2;
	int y = heigth / 2 - imageSide / 2;
	
	Point destinationPoints[] = {
		Point(x, y),
		Point(x+imageSide, y),
		Point(x, y+imageSide)
	};
	pushImage( appState->rotationDegree, destinationPoints);
	graphics.DrawImage(
		&bm,
		destinationPoints, 
		3
	);
}

void onKeyDown(ApplicationState* appState, WPARAM key) {
	if (key == VK_RIGHT) {
		appState->direction = RotationDirection::LEFT;
	
	}
	else if (key == VK_LEFT) {
		appState->direction = RotationDirection::RIGHT;
	}
}

void onKeyUp(ApplicationState* appState, WPARAM key) {
	appState->direction = RotationDirection::NONE;
}

ApplicationState* getAppState(HWND hwnd) {
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	ApplicationState* appstate = reinterpret_cast<ApplicationState*>(ptr);
	return appstate;
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {

	ApplicationState* appState;
	if (uMsg == WM_CREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		ApplicationState* appState = reinterpret_cast<ApplicationState*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)appState);
		return 0;
	}
	else {
		appState = getAppState(hwnd);
	}

	switch (uMsg)
	{
	
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				onPaint(hdc, ps, appState);
				EndPaint(hwnd, &ps);
			}
			return 0;
		case WM_KEYDOWN:
			if (appState->isTimerRunning == 0) {
				appState->isTimerRunning = ANIMATION_TIMER_ID;
				SetTimer(hwnd, ANIMATION_TIMER_ID, 1000/12, NULL);
			}
			onKeyDown(appState, wParam);
			return 0;
		case WM_KEYUP:
			onKeyUp(appState, wParam);
			OutputDebugString(L"FEL");
			if (appState->isTimerRunning != 0) {
				KillTimer(hwnd, ANIMATION_TIMER_ID);
				appState->isTimerRunning = 0;
			}
			return 0;
		case WM_TIMER:
			if (appState->direction == RotationDirection::LEFT)
			{
				appState->rotationDegree -= 5;
			}
			else {
				appState->rotationDegree += 5;
			}
			InvalidateRect(hwnd, NULL, TRUE);
			OutputDebugString(L"lol");
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}