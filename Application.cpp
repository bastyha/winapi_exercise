#include <windows.h>
#include <winuser.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include "Application.h"

PCWSTR Application::ClassName() const {
	return L"Winapi exercise";
}

void Application::onPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	int width = ps.rcPaint.right - ps.rcPaint.left;
	int heigth = ps.rcPaint.bottom - ps.rcPaint.top;

	HDC tempHdc = CreateCompatibleDC(hdc);
	HBITMAP tempBm = CreateCompatibleBitmap(hdc, width, heigth);

	SelectObject(tempHdc, tempBm);

	Gdiplus::Graphics graphics(tempHdc);
	FillRect(tempHdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));
	Gdiplus::Bitmap bm(L"earth.png");


	int imageSide = min(width, heigth);

	graphics.TranslateTransform(width / 2, heigth / 2);
	graphics.RotateTransform(rotation, Gdiplus::MatrixOrderPrepend);

	graphics.DrawImage(
		&bm,
		-imageSide / 2, -imageSide / 2,
		imageSide, imageSide
	);

	BitBlt(hdc, 0, 0, width, heigth, tempHdc, 0, 0, SRCCOPY);
	DeleteObject(tempBm);
	DeleteDC(tempHdc);

	EndPaint(hwnd, &ps);
}

void Application::onTimerExpire() {
	InvalidateRect(hwnd, NULL, TRUE);
}

void Application::onKeyDown(WPARAM key) {
	if (timerState == TimerState::STOPPED) {
		timerState = TimerState::RUNNING;
		SetTimer(hwnd, timerState, 1000 / NUMBER_OF_FRAMES, 0);
	}
	switch (key) {
	case VK_RIGHT:
		rotationDirection = RotationDirection::RIGHT;
		rotation += INCREMENT;
		break;
	case VK_LEFT:
		rotationDirection = RotationDirection::LEFT;
		rotation -= INCREMENT;
		break;
	}
}

void Application::onKeyUp(WPARAM key) {
	if (key == VK_RIGHT
		&& rotationDirection == RotationDirection::RIGHT
		&& timerState == TimerState::RUNNING) {
		KillTimer(hwnd, timerState);
		timerState = TimerState::STOPPED;
	}
	else if (key == VK_LEFT
		&& rotationDirection == RotationDirection::LEFT
		&& timerState == TimerState::RUNNING) {
		KillTimer(hwnd, timerState);
		timerState = TimerState::STOPPED;
	}
}

LRESULT Application::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		onPaint();
		return 0;
	case WM_KEYDOWN:
		onKeyDown(wParam);
		return 0;
	case WM_KEYUP:
		onKeyUp(wParam);
		return 0;
	case WM_SIZING:
		InvalidateRect(hwnd, NULL, false);
		return 0;
	case WM_TIMER:
		onTimerExpire();
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}
	return TRUE;
}

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Application* pThis = NULL;

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (Application*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->hwnd = hwnd;
	}
	else {
		pThis = (Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (pThis)
	{
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	else {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

HWND Application::getHwnd() const {
	return hwnd;
}

Application::Application(HINSTANCE hInstance)
	: NUMBER_OF_FRAMES(60),
	timerState(TimerState::STOPPED),
	rotationDirection(RotationDirection::NONE),
	rotation(45),
	INCREMENT(5){
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName();

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		ClassName(),
		ClassName(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		this
	);
	if (hwnd == NULL) {
		MessageBox(NULL, L"There was a problem when creating the window", L"Creation error", MB_OK);
	}
}

