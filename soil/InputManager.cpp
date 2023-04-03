#include "InputManager.h"

#include <Windows.h>

void Soil::InputManager::inputThreadLoop() {
	while (this->alive) {
		if (!this->paused) {
			for (char key : this->keysToCheck) {
				if (!this->paused) {
					this->inputBuffer[key] = (0x8000 & GetAsyncKeyState((unsigned char)(key))) != 0;
				}
			}
		}
	}
}

Soil::InputManager::InputManager(std::string keysToCheck) {
	this->inputBuffer = std::vector<bool>(0xFF, false);
	this->inputAccessibleBuffer = std::vector<bool>(0xFF, false);

	this->addKeys(keysToCheck);
	
	this->inputThread = std::thread(&Soil::InputManager::inputThreadLoop, this);
}

Soil::InputManager::~InputManager() {
	this->alive = false;
	this->inputThread.join();
}

void Soil::InputManager::addKeys(std::string keysToAdd) {
	this->paused = true;
	for (char key : keysToAdd) {
		if (std::count(this->keysToCheck.begin(), this->keysToCheck.end(), key) == 0) { // check if we already have this key registered
			this->keysToCheck.push_back(key);
		}
	}
	this->paused = false;
}

void Soil::InputManager::removeKeys(std::string keysToRemove) {
	this->paused = true;
	for (char key : keysToRemove) {
		auto index = std::find(this->keysToCheck.begin(), this->keysToCheck.end(), key); // get the index of the key, if it's registered
		if (index < this->keysToCheck.end()) { 
			this->keysToCheck.erase(index);
		}
	}
	this->paused = false;
}

void Soil::InputManager::getBufferState() {
	for (char key : keysToCheck) {
		this->inputAccessibleBuffer[key] = this->inputBuffer[key];
		this->inputBuffer[key] = false;
	}
}

bool Soil::InputManager::isKeyPressed(char key) {
	return this->inputAccessibleBuffer[key];
}