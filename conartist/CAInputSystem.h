// The CAInputSystem class spawns its own thread which monitors keyboard/mouse button input.
// This allows console/terminal games to read multiple inputs at once, whereas consoles are typically limited to a single input at a time.

#pragma once
#include <thread>
#include <vector>
#include <string>
#include <mutex>

namespace ConArtist {
	class CAInputSystem {
	private:
		std::vector<char> keysToCheck = std::vector<char>();
		std::mutex keysToCheckMutex;
		std::vector<bool> inputBuffer = std::vector<bool>();
		std::vector<bool> inputAccessibleBuffer = std::vector<bool>();
		std::thread inputThread;
		std::mutex aliveMutex;
		bool alive = true;
		std::mutex pausedMutex;
		bool paused = false;

		void inputThreadLoop();

	public:
		CAInputSystem(std::string keysToCheck = std::string());
		~CAInputSystem();
		void addKeys(std::string keysToCheck);
		void removeKeys(std::string keysToCheck);
		void getBufferState();
		void clearBuffer();
		bool isKeyPressed(char key);
		bool isAnyKeyPressed();
		void pause();
		void unpause();
	};
}
