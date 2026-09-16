#include <cstdio>
#include <raylib.h>
#include <cstdlib>

// Window Config //
constexpr int defaultWindowWidth = 800;
constexpr int defaultWindowHeight = 600;
constexpr int windowsFps = 120;
constexpr const char* windowsTitle = "Audio Visualizer";

// Visualizer UI Config //
constexpr int rectWidth = 50;
constexpr int rectHeight = 100;
constexpr Color rectColor = RAYWHITE;

int main() {
	int windowsWidth = defaultWindowWidth;
	int windowsHeight = defaultWindowHeight;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(windowsWidth, windowsHeight, windowsTitle);

	SetTargetFPS(windowsFps);

	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			windowsWidth = GetScreenWidth();
			windowsHeight = GetScreenHeight();

#ifdef _DEBUG
			printf("Window resized to: %d x %d\n", windowsWidth, windowsHeight);
#endif

		}

		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		int centerX = windowsWidth / 2;
		int centerY = windowsHeight / 2;

		DrawRectangle(centerX, centerY, rectWidth, rectHeight, rectColor);

		EndDrawing();
	}

	CloseWindow();

	return EXIT_SUCCESS;
}