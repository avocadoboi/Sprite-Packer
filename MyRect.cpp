#include "MyRect.h"

MyRect::MyRect() {};
MyRect::MyRect(int p_w, int p_h)
	: w(p_w), h(p_h) {};
MyRect::MyRect(int p_x, int p_y, int p_w, int p_h)
	: x(p_x), y(p_y), w(p_w), h(p_h) {};

bool MyRect::getArea() {
	return w*h;
}
bool MyRect::intersects(MyRect other) {
	return x	 < other.x + other.w
		&& x + w > other.x
		&& y	 < other.y + other.h
		&& y + h > other.y;
}
bool MyRect::operator==(MyRect rect) {
	return rect.x == x
		&& rect.y == y
		&& rect.w == w
		&& rect.h == h;
}
bool MyRect::operator!=(MyRect rect) {
	return rect.x != x
		&& rect.y != y
		&& rect.w != w
		&& rect.h != h;
}
void MyRect::operator=(MyRect rect) {
	x = rect.x;
	y = rect.y;
	w = rect.w;
	h = rect.h;
}