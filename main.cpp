#include <cstdio>
#include <raylib.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>

// Window Config //
constexpr int defaultWindowWidth = 800;
constexpr int defaultWindowHeight = 600;
constexpr int windowsFps = 120;
constexpr const char* windowsTitle = "Audio Visualizer";

// Audio Config //
bool isMusicPlaying = false;
const std::vector<std::string> supportedAudioFormats = { ".mp3", ".wav", ".ogg" };
const char* currentMusicFilePath = nullptr;

// Visualizer UI Config //
constexpr int rectWidth = 50;
constexpr int rectHeight = 100;
constexpr Color rectColor = RAYWHITE;

// MainPage UI Config //
constexpr const char* mainPageText = "DRAG AND DROP MUSIC FILE TO PLAY";
constexpr Color mainPageTextColor = RAYWHITE;

bool isSupportedFormat(const std::string& extension) {
	for (const std::string& format : supportedAudioFormats) {
		if (format == extension) {
			return true;
		}
	}
	return false;
}

void LoadMusic(const char* filePath) {
#ifdef _DEBUG
	printf("Loading music file: %s\n", filePath);
#endif


	isMusicPlaying = true;
	currentMusicFilePath = filePath;
}

std::string getFileName(const std::string& path) {
	size_t slash = path.find_last_of("\\/");
	size_t dot = path.find_last_of('.');

	size_t start = (slash == std::string::npos) ? 0 : slash + 1;
	size_t end = (dot == std::string::npos || dot < start) ? path.size() : dot;

	return path.substr(start, end - start);
}

int main() {
	int windowsWidth = defaultWindowWidth;
	int windowsHeight = defaultWindowHeight;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(windowsWidth, windowsHeight, windowsTitle);
	InitAudioDevice();

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
		ClearBackground(DARKGRAY);

		if (isMusicPlaying) {
			const std::string musicFileName = getFileName(currentMusicFilePath);
			const char* text = TextFormat("Now playing: %s", musicFileName.c_str());

			DrawText(text, (windowsWidth - MeasureText(text, 20)) / 2, (windowsHeight - 200) / 2, 20, mainPageTextColor);
		}
		else {
			DrawText(mainPageText, (windowsWidth - MeasureText(mainPageText, 20)) / 2, (windowsHeight - 20) / 2, 20, mainPageTextColor);

			if (IsFileDropped()) {
				FilePathList droppedFiles = LoadDroppedFiles();

				for (unsigned int i = 0; i < droppedFiles.count; i++) {
					const char* path = droppedFiles.paths[i];

					if (FileExists(path) && isSupportedFormat(GetFileExtension(path))) {
						LoadMusic(path);
					}
				}
			}
		}

		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();

	return EXIT_SUCCESS;
}