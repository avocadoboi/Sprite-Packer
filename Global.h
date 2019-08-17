#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "MyRect.h"
using namespace std;
using namespace sf;

extern void saveStringToFile(string content, string filePath);

extern Image fullImage;

extern vector<MyRect> freeRectangles;

extern Vector2i enclosingRectangle;

extern int smallestArea;
extern int smallestWidth;
extern int smallestHeight;
extern int fitMode;
extern int sortMode;