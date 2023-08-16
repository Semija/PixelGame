#pragma once

class Pixel {
public:
	uint_fast8_t r,g,b;

	Pixel(uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue = 0)
	: r(red), g(green), b(blue) {}

	Pixel(uint_fast8_t all = 0)
		: r(all), g(all), b(all) {}

	void Set(uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue = 0) {
		r = red;
		g = green;
		b = blue;
	}

	void Set(uint_fast8_t all = 0) {
		r = all;
		g = all;
		b = all;
	}
};

#pragma region Colors
/* Constant of Pixel Colors */
static const Pixel 

GREY				(192, 192, 192),
DARK_GREY			(128, 128, 128),
VERY_DARK_GREY		(64, 64, 64),

RED					(255, 0, 0),
DARK_RED			(128, 0, 0),
VERY_DARK_RED		(64, 0, 0),

YELLOW				(255, 255, 0),
DARK_YELLOW			(128, 128, 0),
VERY_DARK_YELLOW	(64, 64, 0),

GREEN				(0, 255, 0),
DARK_GREEN			(0, 128, 0),
VERY_DARK_GREEN		(0, 64, 0),

CYAN				(0, 255, 255),
DARK_CYAN			(0, 128, 128),
VERY_DARK_CYAN		(0, 64, 64),

BLUE				(0, 0, 255),
DARK_BLUE			(0, 0, 128),
VERY_DARK_BLUE		(0, 0, 64),

MAGENTA				(255, 0, 255),
DARK_MAGENTA		(128, 0, 128),
VERY_DARK_MAGENTA	(64, 0, 64),

WHITE				(255, 255, 255),
BLACK				(0, 0, 0);

#pragma endregion