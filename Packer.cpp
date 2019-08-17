#include "Packer.h"

Packer::Packer() {
	freeRectangles.push_back(MyRect(0, 0));
}

bool Packer::getWillFirstImageRectBeFirst(ImageRect a, ImageRect b) {
	if (sortMode == 0) {
		if (a.getRect().getArea() > b.getRect().getArea()) {
			return true;
		}
		else if (a.getRect().getArea() < b.getRect().getArea()) {
			return false;
		}
		else {
			if (a.getRect().w > b.getRect().w) {
				return true;
			}
			else if (a.getRect().w < b.getRect().w) {
				return false;
			}
			else if (a.getRect().h > b.getRect().h) {
				return true;
			}
			else return false;
		}
	}
	else if (sortMode == 1) {
		if (a.getRect().w > b.getRect().w) {
			return true;
		}
		else if (a.getRect().w < b.getRect().w) {
			return false;
		}
		else if (a.getRect().h > b.getRect().h) {
			return true;
		}
		else return false;
	}
	else if (sortMode == 2) {
		if (a.getRect().h > b.getRect().h) {
			return true;
		}
		else if (a.getRect().h < b.getRect().h) {
			return false;
		}
		else if (a.getRect().w > b.getRect().w) {
			return true;
		}
		else return false;
	}
}
void Packer::sortImageRects() {
	sort(imageRects.begin(), imageRects.end(), getWillFirstImageRectBeFirst);
}

bool Packer::openFiles() {
	RenderWindow window(VideoMode(0, 0), "meow", Style::None);

	IFileOpenDialog* fileDialog;
	CoCreateInstance(
		CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&fileDialog)
	);

	COMDLG_FILTERSPEC fileTypes[] = {
		{ L"png", L"*.png" },
		{ L"jpg", L"*.jpg" },
		{ L"All", L"*.*" }
	};
	fileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	fileDialog->SetFileTypeIndex(0);
	fileDialog->SetDefaultExtension(L"png");

	FILEOPENDIALOGOPTIONS options;
	fileDialog->GetOptions(&options);
	fileDialog->SetOptions(options | FOS_ALLOWMULTISELECT);
	fileDialog->SetOkButtonLabel(L"PACK!!");

	fileDialog->Show(NULL);

	IShellItemArray* results;
	if (SUCCEEDED(fileDialog->GetResults(&results))) {
		DWORD numberOfResultItems;
		results->GetCount(&numberOfResultItems);
		IShellItem* currentResultItem;
		LPWSTR currentFilePath;
		for (int c = 0; c < (int)numberOfResultItems; c++) {
			results->GetItemAt(c, &currentResultItem);
			currentResultItem->GetDisplayName(SIGDN_FILESYSPATH, &currentFilePath);
			imageRects.push_back(ImageRect((string)CW2A(currentFilePath)));
		}
		return true;
	}
	return false;
}
void Packer::exportRects() {
	string headerString =
		"#pragma once\n"
		"#include <SFML/Graphics.hpp>\n"
		"#include <string>\n\n"
		"class TextureRects{\n"
		"public:\n";
	string sourceString =
		"#include \"TextureRects.hpp\"\n\n";
	for (int c = 0; c < (int)imageRects.size(); c++) {
		headerString += "\tstatic sf::IntRect " + imageRects[c].getName() + ";\n";

		MyRect rect = imageRects[c].getRect();
		sourceString += "sf::IntRect TextureRects::" + imageRects[c].getName() + " = sf::IntRect("
			+ to_string(rect.x) + ", " + to_string(rect.y) + ", "
			+ to_string(rect.w) + ", " + to_string(rect.h) + ");\n";
	}
	headerString += "\n\tstatic sf::IntRect getTextureRectFromName(const std::string& p_name);\n";
	headerString += "};";

	sourceString += "\nsf::IntRect TextureRects::getTextureRectFromName(const std::string& p_name) {\n";
	for (int c = 0; c < (int)imageRects.size(); c++) {
		sourceString += "\tif (p_name == \"" + imageRects[c].getName() + "\") " + "return TextureRects::" + imageRects[c].getName() + ";\n";
	}
	sourceString += "\treturn IntRect();\n};";

	saveStringToFile(headerString, "TextureRects.hpp");
	saveStringToFile(sourceString, "TextureRects.cpp");
}

bool Packer::findPositions() {
	freeRectangles = {};
	freeRectangles.push_back(MyRect(0, 0, enclosingRectangle.x, enclosingRectangle.y));
	for (int a = 0; a < imageRects.size(); a++) {
		if (!imageRects[a].findPosition()) {
			return false;
		}
	}

	int area = enclosingRectangle.x*enclosingRectangle.y;
	if (recordArea == 0 || area < recordArea) {
		recordArea = area;
		recordEnclosingRectangle = enclosingRectangle;
		recordFitMode = fitMode;
		recordSortMode = sortMode;
	}
	return true;
}
void Packer::pack() {
	for (int i_sortMode = 0; i_sortMode <= 2; i_sortMode++) {
		sortMode = i_sortMode;
		sortImageRects();

		for (int i_fitMode = 0; i_fitMode <= 5; i_fitMode++) {
			fitMode = i_fitMode;

			enclosingRectangle.x = totalWidth;
			enclosingRectangle.y = biggestHeight;
			findPositions();

			int rightestRight = 0;
			for (auto c : imageRects) {
				if (c.getRect().x + c.getRect().w > rightestRight) {
					rightestRight = c.getRect().x + c.getRect().w;
				}
			}
			enclosingRectangle.x = rightestRight;

			while (true) {
				if (findPositions()) {
					enclosingRectangle.x = recordArea / enclosingRectangle.y - 1;
					if (enclosingRectangle.x < biggestWidth) break;
				}
				else {
					enclosingRectangle.y += 1;
				}
				if (enclosingRectangle.x*enclosingRectangle.y < totalArea) {
					enclosingRectangle.y = totalArea / enclosingRectangle.x;
				}
			}
		}
	}
}
bool Packer::save() {
	enclosingRectangle = recordEnclosingRectangle;
	sortMode = recordSortMode;
	fitMode = recordFitMode;
	sortImageRects();

	cout << (findPositions() ? "Success!" : "Failed. (this shouldn't happen...)") << endl;

	fullImage.create(enclosingRectangle.x, enclosingRectangle.y, Color::Transparent);
	for (auto imageRect : imageRects) {
		imageRect.copyToMainImage();
	}

	/*for (int i_freeRect = 0; i_freeRect < freeRectangles.size(); i_freeRect++) {
		for (int x = freeRectangles[i_freeRect].x; x < freeRectangles[i_freeRect].x + freeRectangles[i_freeRect].w; x++) {
			fullImage.setPixel(x, freeRectangles[i_freeRect].y, Color::Red);
			fullImage.setPixel(x, freeRectangles[i_freeRect].y + freeRectangles[i_freeRect].h - 1, Color::Red);
		}
		for (int y = freeRectangles[i_freeRect].y; y < freeRectangles[i_freeRect].y + freeRectangles[i_freeRect].h; y++) {
			fullImage.setPixel(freeRectangles[i_freeRect].x, y, Color::Red);
			fullImage.setPixel(freeRectangles[i_freeRect].x + freeRectangles[i_freeRect].w - 1, y, Color::Red);
		}
	}*/

	if (!fullImage.saveToFile("result.png")) {
		return false;
	}

	exportRects();

	return true;
}

void Packer::run() {
	if (!openFiles()) {
		return;
	}
	for (int c = 0; c < imageRects.size(); c++) {
		MyRect rect = imageRects[c].getRect();
		int area = rect.w*rect.h;
		totalArea += area;
		totalWidth += rect.w;
		if (rect.w > biggestWidth) {
			biggestWidth = rect.w;
		}
		if (rect.h > biggestHeight) {
			biggestHeight = rect.h;
		}
		if (area < smallestArea || smallestArea == 0) {
			smallestArea = area;
		}
		if (rect.w < smallestWidth || smallestWidth == 0) {
			smallestWidth = rect.w;
		}
		if (rect.h < smallestHeight || smallestHeight == 0) {
			smallestHeight = rect.h;
		}
	}

	cout << "Packing..." << endl;
	pack();
	if (save()) {
		cout
			<< "\nResult:\n"
			<< "  area:   " << recordArea << endl
			<< "  Width:  " << enclosingRectangle.x << endl
			<< "  Height: " << enclosingRectangle.y << endl;
	}
	else {
		cout << "\nClose any programs that are using the files and try again." << endl;
	}
}