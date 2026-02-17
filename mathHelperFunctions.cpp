#include <cmath>
#include <windows.h>
#include <winuser.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")



double degToRad(double deg) {
	double pi = 3.14159265359;
	return deg * (pi / 180);
}

void rotateRectangleAroundCenter(double radius, double degree, Gdiplus::Point (&points)[3]) {
	double sinCoord = std::sin(degToRad(degree)) * radius;
	double cosCoord = std::cos(degToRad(degree)) * radius;
	points[0] = Gdiplus::Point(-1 * sinCoord, cosCoord);
	points[1] = Gdiplus::Point(cosCoord, sinCoord);
	points[2] = Gdiplus::Point(sinCoord, -1 * cosCoord);
}

void pushImage(int amount, Gdiplus::Point (&points)[3]) {
	points[0] = points[0] - Gdiplus::Point(amount, 0);
	points[1] = points[1] - Gdiplus::Point(amount, 0);
	points[2] = points[2] - Gdiplus::Point(amount, 0);
}