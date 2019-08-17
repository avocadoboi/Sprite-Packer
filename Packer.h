#pragma once
#include <ShObjIdl.h>
#include <atlstr.h>
#include "ImageRect.h"
#include <algorithm>
#include <vector>
#include <set>

class Packer{
private:
	static bool getWillFirstImageRectBeFirst(ImageRect a, ImageRect b);
	void sortImageRects();
	bool openFiles();
	void exportRects();

	bool findPositions();
	void pack();
	bool save();


	Vector2i recordEnclosingRectangle;

	int recordArea = 0;
	int recordFitMode = 0;
	int recordSortMode = 0;
	int totalArea = 0;
	int totalWidth = 0;
	int biggestWidth = 0;
	int biggestHeight = 0;

public:
	Packer();

	void run();
};