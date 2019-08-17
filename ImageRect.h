#pragma once
#include <Windows.h>
#include "Global.h"

class ImageRect{
private:
	Image image;

	string name;

	MyRect rect;

public:
	ImageRect(string filePath);

	void copyToMainImage();
	void setPosition(Vector2i position);
	void setPosition(int x, int y);

	string getName();

	MyRect getRect();

	bool findPosition();

	Vector2i recordPosition;
};

extern vector<ImageRect> imageRects;