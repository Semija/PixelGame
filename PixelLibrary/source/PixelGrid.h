#pragma once

#include "Pixel.h"



class PixelGrid {
public:
    int width = 64;
    int height = 64;
    int pixelsCount = width * height;
	GLubyte *pixels; // Array size is width * height * 3 (1 byte per channel)

    void Fill(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b) {
        for (int i = 0; i < (pixelsCount*3); i += 3) {
            pixels[i    ] = r;
            pixels[i + 1] = g;
            pixels[i + 2] = b;
        }
    }

    void Fill(Pixel pixel) {
        for (int i = 0; i < (pixelsCount*3); i += 3) {
            pixels[i] = pixel.r;
            pixels[i + 1] = pixel.g;
            pixels[i + 2] = pixel.b;
        }
    }

    void Clear() {
        Fill(BLACK);
    }


    PixelGrid() {
        pixels = new GLubyte[width * height * 3];
        Clear();
    }

    PixelGrid(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
        pixels = new GLubyte[width * height * 3];
        Clear();
    }

    ~PixelGrid() {
        delete[] pixels;
    }


    Pixel GetPixel(int x, int y) {
        Pixel pixel = Pixel(0, 0, 0);

        uint32_t pixelIndex = ((y * width) + x) * 3;

        pixel.r = (uint_fast8_t)pixels[pixelIndex];
        pixel.g = (uint_fast8_t)pixels[pixelIndex + 1];
        pixel.b = (uint_fast8_t)pixels[pixelIndex + 2];

        return pixel;
    }

	void ChangeSize(int newWidth, int newHeight) {
		delete [] pixels;
        width = newWidth;
        height = newHeight;
        pixelsCount = newWidth * newHeight;
		pixels = new GLubyte[newWidth * newHeight * 3];
	}

	void Draw(int x, int y, uint_fast8_t r, uint_fast8_t g, uint_fast8_t b) {
		if (x >= width || y >= height || x < 0 || y < 0) {
			return;
		}

		uint_fast32_t pixelIndex = ((y * width) + x) * 3;

		pixels[pixelIndex] = r;
		pixels[pixelIndex + 1] = g;
		pixels[pixelIndex + 2] = b;
	}

	void Draw(int x, int y, Pixel pixel) {
		Draw(x, y, pixel.r, pixel.g, pixel.b);
	}

    void DrawLine(int x1, int y1, int x2, int y2, Pixel p) {
        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
        dx = x2 - x1; dy = y2 - y1;

        // straight lines
        if (dx == 0) { // Line is vertical
            if (y2 < y1) std::swap(y1, y2);
            for (y = y1; y <= y2; y++) Draw(x1, y, p);
            return;
        }

        if (dy == 0) { // Line is horizontal
            if (x2 < x1) std::swap(x1, x2);
            for (x = x1; x <= x2; x++) Draw(x, y1, p);
            return;
        }

        // Line is Funky
        dx1 = abs(dx); dy1 = abs(dy);
        px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
        if (dy1 <= dx1) {
            if (dx >= 0) { x = x1; y = y1; xe = x2; }
            else         { x = x2; y = y2; xe = x1; }

            Draw(x, y, p);

            for (i = 0; x < xe; i++) {
                x = x + 1;
                if (px < 0) { px = px + 2 * dy1; }
                else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
                    px = px + 2 * (dy1 - dx1);
                }
                Draw(x, y, p);
            }
        }
        else {
            if (dy >= 0) { x = x1; y = y1; ye = y2; }
            else         { x = x2; y = y2; ye = y1; }

            Draw(x, y, p);

            for (i = 0; y < ye; i++) {
                y = y + 1;
                if (py <= 0) { py = py + 2 * dx1; }
                else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
                    py = py + 2 * (dx1 - dy1);
                }
                Draw(x, y, p);
            }
        }
    }



};