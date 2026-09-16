#include <stdio.h>
#include <raylib.h>

// Window Config //
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FPS 120
#define WINDOW_TITLE "Audio Visualizer"

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	SetTargetFPS(WINDOW_FPS);

	if (!WindowShouldClose()) {
		
	}

	return 0;
}