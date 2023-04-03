#pragma once
#include <thread>
#include <vector>
#include <string>

namespace Soil {
	class InputManager {
	private:
		std::vector<char> keysToCheck = std::vector<char>();
		std::vector<bool> inputBuffer = std::vector<bool>();
		std::vector<bool> inputAccessibleBuffer = std::vector<bool>();
		std::thread inputThread;
		bool alive = true;
		bool paused = false;

		void inputThreadLoop();

	public:
		InputManager(std::string keysToCheck = std::string());
		~InputManager();
		void addKeys(std::string keysToCheck);
		void removeKeys(std::string keysToCheck);
		void getBufferState();
		bool isKeyPressed(char key);
	};
}
