#include "Global.h"

void saveStringToFile(string content, string filePath) {
	ofstream fileStream;
	fileStream.open(filePath);
	fileStream << content;
	fileStream.close();
}

Image fullImage;

vector<MyRect> freeRectangles;

Vector2i enclosingRectangle(0, 0);

int currentRectIndex = 0;
int smallestArea = 0;
int smallestWidth = 0;
int smallestHeight = 0;
int fitMode = 0;
int sortMode = 0;