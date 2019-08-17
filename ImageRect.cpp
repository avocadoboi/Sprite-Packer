#include "ImageRect.h"

ImageRect::ImageRect(string filePath) {
	image.loadFromFile(filePath);
	rect.w = image.getSize().x;
	rect.h = image.getSize().y;
	for (int c = 0; c < filePath.size(); c++) {
		if (filePath[c] == '\\') {
			filePath.erase(filePath.begin(), filePath.begin() + c + 1);
			c = 0;
		}
		if (filePath[c] == '.') {
			filePath.erase(filePath.begin() + c, filePath.end());
			c = filePath.size() - 1;
		}
	}
	name = filePath;
}

void ImageRect::copyToMainImage() {
	for (int x = rect.x; x < rect.x + rect.w && x < fullImage.getSize().x; x++) {
		for (int y = rect.y; y < rect.y + rect.h && y < fullImage.getSize().y; y++) {
			Color nextColor = image.getPixel(x - rect.x, y - rect.y);
			fullImage.setPixel(x, y, nextColor);
		}
	}
}
void ImageRect::setPosition(Vector2i position) {
	rect.x = position.x;
	rect.y = position.y;
}
void ImageRect::setPosition(int x, int y) {
	setPosition(Vector2i(x, y));
}

string ImageRect::getName() {
	return name;
}

MyRect ImageRect::getRect() {
	return rect;
}

bool ImageRect::findPosition() {
	int bestRectIndex = -1;
	int bestScore = 0;
	for (int a = 0; a < freeRectangles.size(); a++) {
		// Try the position of this free rectangle.
		// If it fits, update the highscore.
		if (rect.w <= freeRectangles[a].w &&
			rect.h <= freeRectangles[a].h) {
			int score;
			switch (fitMode) {
			case 0:
				score = freeRectangles[a].x*freeRectangles[a].x + freeRectangles[a].y*freeRectangles[a].y;
				break;
			case 1:
				score = freeRectangles[a].w*freeRectangles[a].h;
				break;
			case 2:
				score = freeRectangles[a].x;
				break;
			case 3:
				score = freeRectangles[a].y;
				break;
			case 4:
				score = freeRectangles[a].w;
				break;
			case 5:
				score = freeRectangles[a].h;
				break;
			}
			if (score < bestScore || bestScore == 0) {
				bestScore = score;
				bestRectIndex = a;
			}
		}
	}
	if (bestRectIndex == -1) return false;

	setPosition(freeRectangles[bestRectIndex].x, freeRectangles[bestRectIndex].y);

	// Check for free rectangles that intersect 
	// with this imagerect and split them
	int numberOfNewFreeRectangles = 0;
	for (int a = 0; a < freeRectangles.size(); a++) {
		if (rect.x < freeRectangles[a].x + freeRectangles[a].w && rect.x + rect.w > freeRectangles[a].x) {
			// New free rectangle at the top of this imagerect
			if (rect.y > freeRectangles[a].y && rect.y < freeRectangles[a].y + freeRectangles[a].h) {
				freeRectangles.push_back(MyRect(
					freeRectangles[a].x,
					freeRectangles[a].y,
					freeRectangles[a].w,
					rect.y - freeRectangles[a].y
				));
				numberOfNewFreeRectangles++;
			}
			// New free rectangle at the bottom of this imagerect
			if (rect.y + rect.h < freeRectangles[a].y + freeRectangles[a].h && rect.y + rect.h > freeRectangles[a].y) {
				freeRectangles.push_back(MyRect(
					freeRectangles[a].x,
					rect.y + rect.h,
					freeRectangles[a].w,
					freeRectangles[a].y + freeRectangles[a].h - (rect.y + rect.h)
				));
				numberOfNewFreeRectangles++;
			}
		}
		if (rect.y < freeRectangles[a].y + freeRectangles[a].h && rect.y + rect.h > freeRectangles[a].y) {
			// New free rectangle at the left of this imagerect
			if (rect.x > freeRectangles[a].x && rect.x < freeRectangles[a].x + freeRectangles[a].w) {
				freeRectangles.push_back(MyRect(
					freeRectangles[a].x,
					freeRectangles[a].y,
					rect.x - freeRectangles[a].x,
					freeRectangles[a].h
				));
				numberOfNewFreeRectangles++;
			}
			// New free rectangle at the right of this imagerect
			if (rect.x + rect.w < freeRectangles[a].x + freeRectangles[a].w && rect.x + rect.w > freeRectangles[a].x) {
				freeRectangles.push_back(MyRect(
					rect.x + rect.w,
					freeRectangles[a].y,
					freeRectangles[a].x + freeRectangles[a].w - (rect.x + rect.w),
					freeRectangles[a].h
				));
				numberOfNewFreeRectangles++;
			}
		}
		if (freeRectangles[a].intersects(rect)) {
			freeRectangles.erase(freeRectangles.begin() + a);
			a--;
		}
	}

	// Prune!
	for (int a = freeRectangles.size() - numberOfNewFreeRectangles; a < freeRectangles.size(); a++) {
		if (
			freeRectangles[a].w*freeRectangles[a].h < smallestArea ||
			freeRectangles[a].w < smallestWidth ||
			freeRectangles[a].h < smallestHeight
			) {
			freeRectangles.erase(freeRectangles.begin() + a);
			if (a == freeRectangles.size()) return true;
			a--;
			continue;
		}
		for (int b = 0; b < freeRectangles.size(); b++) {
			if (b == a) continue;
			if (
				freeRectangles[b].intersects(freeRectangles[a]) &&
				(freeRectangles[b].x == freeRectangles[a].x || freeRectangles[b].y == freeRectangles[a].y)
				) {
				if (
					freeRectangles[a].w <= freeRectangles[b].w &&
					freeRectangles[a].h <= freeRectangles[b].h
					) {
					freeRectangles.erase(freeRectangles.begin() + a);
					if (b > a) b--;
					a--;
				}
				else if (
					freeRectangles[b].w <= freeRectangles[a].w &&
					freeRectangles[b].h <= freeRectangles[a].h
					) {
					freeRectangles.erase(freeRectangles.begin() + b);
					if (a > b) a--;
					b--;
				}
			}
		}
	}
	return true;
}

vector<ImageRect> imageRects;