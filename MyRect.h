#pragma once

struct MyRect {
	MyRect();
	MyRect(int p_w, int p_h);
	MyRect(int p_x, int p_y, int p_w, int p_h);

	bool getArea();
	bool intersects(MyRect other);
	bool operator==(MyRect rect);
	bool operator!=(MyRect rect);
	void operator=(MyRect rect);

	int x = 0, y = 0, w = 0, h = 0;
};