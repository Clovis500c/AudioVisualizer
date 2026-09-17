#include <cstdio>
#include <raylib.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>
#include <format>
#include <cmath>

// Window Config //
constexpr int defaultWindowWidth = 800;
constexpr int defaultWindowHeight = 600;
constexpr int windowsFps = 120;
constexpr const char* windowsTitle = "Audio Visualizer";

// Audio Config //
bool isMusicPlaying = false;
const std::vector<std::string> supportedAudioFormats = { ".mp3", ".wav", ".ogg" };
const char* currentMusicFilePath = nullptr;
constexpr float MINUTE = 60.0f;
float currentVolume = 0.0f;
int currentAudioChannels = 2; // defaultValue

std::vector<int> audioElementsSizes = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Example sizes for audio elements

// Visualizer UI Config //
constexpr int visualizerElementWidth = 10;
constexpr int visualizerElementHeight = 160;
constexpr int visualizerElementPadding = 5;
constexpr int visualizerElementTopY = 60;
constexpr Color rectColor = RAYWHITE;

// MainPage UI Config //
constexpr const char* mainPageText = "DRAG AND DROP MUSIC FILE TO PLAY";
constexpr Color mainPageTextColor = RAYWHITE;

// Functions //
bool isSupportedFormat(const std::string& extension) {
	for (const std::string& format : supportedAudioFormats) {
		if (format == extension) {
			return true;
		}
	}
	return false;
}

Music LoadMusic(const char* filePath) {
#ifdef _DEBUG
	printf("Loading music file: %s\n", filePath);
#endif

	Music music = LoadMusicStream(filePath);
	PlayMusicStream(music);

	if (IsMusicValid(music)) {
		isMusicPlaying = true;
		currentMusicFilePath = filePath;
		currentAudioChannels = music.stream.channels;
	}
	else {
		isMusicPlaying = false;
		currentMusicFilePath = nullptr;

		printf("Failed to load music file: %s\n", filePath);
	}

	return music;
}

std::string getFileName(const std::string& path) {
	size_t slash = path.find_last_of("\\/");
	size_t dot = path.find_last_of('.');

	size_t start = (slash == std::string::npos) ? 0 : slash + 1;
	size_t end = (dot == std::string::npos || dot < start) ? path.size() : dot;

	return path.substr(start, end - start);
}

std::string formatTime(float seconds) {
	int total = (int)seconds;
	return std::format("{:02}:{:02}", total / 60, total % 60);
}

void audioStreamCallback(void *bufferData, unsigned int frames)
{
    float *buffer = (float *)bufferData; // cast to actual sample type

	int totalElements = audioElementsSizes.size();
	int totalValue = frames * currentAudioChannels;
	float sum = 0.0f;

	for (int i = 0; i < totalValue; i++) {
		sum += std::fabs(buffer[i]);
	}
	
	currentVolume = sum / totalValue;

	return;
}

int elementsPossible(int windowsWidth) {
	return (windowsWidth - visualizerElementPadding) / (visualizerElementWidth + visualizerElementPadding);
}

void updateElementsSizes(int newSize) {
	if (audioElementsSizes.size() > newSize) {

	}
	else if (audioElementsSizes.size() < newSize) {

	}
}

// Main //
int main() {
	int windowsWidth = defaultWindowWidth;
	int windowsHeight = defaultWindowHeight;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(windowsWidth, windowsHeight, windowsTitle);
	InitAudioDevice();

	SetTargetFPS(windowsFps);

	Music music = {};

	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			windowsWidth = GetScreenWidth();
			windowsHeight = GetScreenHeight();

#ifdef _DEBUG
			printf("Window resized to: %d x %d\n", windowsWidth, windowsHeight);
			printf("Total elements possible	: %d\n", elementsPossible(windowsWidth));
#endif

		}

		BeginDrawing();
		ClearBackground(DARKGRAY);

		if (isMusicPlaying) {
			UpdateMusicStream(music);

			// Music Title
			const std::string musicFileName = getFileName(currentMusicFilePath);
			const std::string musicTimePlayedText = formatTime(GetMusicTimePlayed(music));
			const std::string musicLengthText = formatTime(GetMusicTimeLength(music));

			const char* text = TextFormat("%s - %s / %s", musicFileName.c_str(), musicTimePlayedText.c_str(), musicLengthText.c_str());

			DrawText(text, (windowsWidth - MeasureText(text, 20)) / 2, (windowsHeight - 200) / 2, 20, mainPageTextColor);

			// Bars
			Color elementColor = RAYWHITE; // temporary

			int totalElements = elementsPossible(windowsWidth);
			int elementsWidth = totalElements * visualizerElementWidth;
			int gapsWdith = visualizerElementPadding * (totalElements - 1);
			int spaceWidth = windowsWidth - elementsWidth - gapsWdith;
			int startX = spaceWidth / 2;
			int baseY = windowsHeight / 2 + 60;

			for (int i = 0; i < totalElements; i++) {
				int posX = i * (visualizerElementWidth + visualizerElementPadding) + startX;

				float centerIndex = totalElements / 2.0f;
				float distance = std::fabs(i - centerIndex);
				float centerProximity = 1.0f - (distance / centerIndex);
				float elementHeight = (float)std::fabs((currentVolume * visualizerElementHeight) * centerProximity);

				float posY = baseY - elementHeight;

				printf("%f\n",elementHeight);

				DrawRectangle(posX, posY, visualizerElementWidth, elementHeight, elementColor);
			}
		}
		else {
			DrawText(mainPageText, (windowsWidth - MeasureText(mainPageText, 20)) / 2, (windowsHeight - 20) / 2, 20, mainPageTextColor);

			if (IsFileDropped()) {
				FilePathList droppedFiles = LoadDroppedFiles();

				for (unsigned int i = 0; i < droppedFiles.count; i++) {
					const char* path = droppedFiles.paths[i];

					if (FileExists(path) && isSupportedFormat(GetFileExtension(path))) {
						music = LoadMusic(path);
						AttachAudioStreamProcessor(music.stream, audioStreamCallback);
					}
				}
			}
		}

		EndDrawing();
	}

	UnloadMusicStream(music);

	CloseWindow();
	CloseAudioDevice();

	return EXIT_SUCCESS;
}