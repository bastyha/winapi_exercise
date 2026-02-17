#include <windows.h>
#include <winuser.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#include <cmath>

#include "helperClasses.cpp"

class Application {
private:
	HWND hwnd;

	HDC hdc;
	PAINTSTRUCT ps;

	bool firstPaint = true;
	TimerState timerState = TimerState::STOPPED;
	RotationDirection rotationDirection = RotationDirection::NONE;


	PCWSTR ClassName() const {
		return L"Winapi exercise";
	}


	void onPaint() {
		if (firstPaint)
		{
			hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));
			firstPaint = false;
		}
	}

	void onKeyDown(WPARAM key) {
		if (timerState == TimerState::STOPPED) {
			timerState = TimerState::RUNNING;
			SetTimer(hwnd, timerState, 0, 0);
		}

		switch (key) {
		case VK_RIGHT:
			rotationDirection = RotationDirection::RIGHT;
			break;
		case VK_LEFT:
			rotationDirection = RotationDirection::LEFT;
			break;
		}
	}

	void onKeyUp(WPARAM key) {
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

public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

	HWND getHwnd()const {
		return hwnd;
	}


	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch(uMsg){
		case WM_DESTROY:
			EndPaint(hwnd, &ps);
			PostQuitMessage(0);
			return 0;
		case WM_CREATE:
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
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		
		}
		return TRUE;
	}


	Application(HINSTANCE hInstance) {
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
			//throw
		}
	}

};