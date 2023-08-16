#include "PixelEngine.h"



void Start() {
	SetVsync(true);
}

void Update() {
	// MainGrid.Clear();

	if (IsMousePressed(MOUSE_LEFT)) {
		MainGrid.Draw(GetMouseX(), GetMouseY(), GREEN);
	}

	if (IsMousePressed(MOUSE_RIGHT)) {
		MainGrid.Draw(GetMouseX(), GetMouseY(), BLACK);
	}

	SetTitle(GetFPSinString());
}



void main() {
	CreateWindow(Start, Update);
}