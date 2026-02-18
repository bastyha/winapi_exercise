#ifndef APPLICATION_INCLUDED
#define APPLICATION_INCLUDED

#include <windows.h>
#include <gdiplus.h>

enum RotationDirection {
	LEFT = -1,
	NONE = 0,
	RIGHT = 1,
};

enum TimerState {
	STOPPED = 0,
	RUNNING = 1, // Used as timer id
};


class Application {
private:
	HWND hwnd;
	const int NUMBER_OF_FRAMES;
	bool firstPaint;
	TimerState timerState;
	RotationDirection rotationDirection;
	int rotation;

	PCWSTR ClassName() const;
	void onPaint();
	void onTimerExpire();
	void onKeyDown(WPARAM key);
	void onKeyUp(WPARAM key);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND getHwnd() const;
	Application(HINSTANCE hInstance);
};

#endif // !APPLICATION_INCLUDED

