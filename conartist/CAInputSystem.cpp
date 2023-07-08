#include "CAInputSystem.h"

#include <Windows.h>

void ConArtist::CAInputSystem::inputThreadLoop() {
	while (this->alive) {
		this->keysToCheckMutex.lock();
		for (char key : this->keysToCheck) {
			this->inputBuffer[key] = this->inputBuffer[key] || (0x8000 & GetAsyncKeyState((unsigned char)(key))) != 0;
		}
		this->keysToCheckMutex.unlock();
	}
}

ConArtist::CAInputSystem::CAInputSystem(std::string keysToCheck) {
	this->inputBuffer = std::vector<bool>(0xFF, false);
	this->inputAccessibleBuffer = std::vector<bool>(0xFF, false);

	this->addKeys(keysToCheck);
	
	this->inputThread = std::thread(&ConArtist::CAInputSystem::inputThreadLoop, this);
}

ConArtist::CAInputSystem::~CAInputSystem() {
	this->alive = false;
	this->inputThread.join();
}

void ConArtist::CAInputSystem::addKeys(std::string keysToAdd) {
	this->keysToCheckMutex.lock();
	for (char key : keysToAdd) {
		if (std::count(this->keysToCheck.begin(), this->keysToCheck.end(), key) == 0) { // check if we already have this key registered
			this->keysToCheck.push_back(key);
		}
	}
	this->keysToCheckMutex.unlock();
}

void ConArtist::CAInputSystem::removeKeys(std::string keysToRemove) {
	this->keysToCheckMutex.lock();
	for (char key : keysToRemove) {
		auto index = std::find(this->keysToCheck.begin(), this->keysToCheck.end(), key); // get the index of the key, if it's registered
		if (index < this->keysToCheck.end()) { 
			this->keysToCheck.erase(index);
		}
	}
	this->keysToCheckMutex.unlock();
}

void ConArtist::CAInputSystem::getBufferState() {
	for (char key : keysToCheck) {
		this->inputAccessibleBuffer[key] = this->inputBuffer[key];
		this->inputBuffer[key] = false;
	}
}

bool ConArtist::CAInputSystem::isKeyPressed(char key) {
	return this->inputAccessibleBuffer[key];
}