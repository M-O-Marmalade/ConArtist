#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <map>
#include <string>
#include <vector>

#include "AudioSystem.h"


Soil::AudioSystem::AudioSystem(bool fmodLiveUpdate) {

	// create the Studio System
	fmodResult = FMOD::Studio::System::create(&fmodSystem);

	// configure Studio System for stereo output
	FMOD::System* fmodCoreSystem;
	fmodSystem->getCoreSystem(&fmodCoreSystem);
	int sampleRate;
	FMOD_SPEAKERMODE speakerMode;
	int numRawSpeakers;
	fmodCoreSystem->getSoftwareFormat(&sampleRate, &speakerMode, &numRawSpeakers);
	fmodCoreSystem->setSoftwareFormat(sampleRate, FMOD_SPEAKERMODE_STEREO, numRawSpeakers);

	// initialize the Studio System
	fmodResult = fmodSystem->initialize(256, fmodLiveUpdate ? FMOD_STUDIO_INIT_LIVEUPDATE : FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
}

Soil::AudioSystem::~AudioSystem() {
	this->fmodSystem->release();
}

void Soil::AudioSystem::loadMasterBank(std::string bankPath) {
	fmodSystem->loadBankFile( bankPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &(this->masterBank));
}

void Soil::AudioSystem::loadStringsBank(std::string bankPath) {
	fmodSystem->loadBankFile( bankPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &(this->stringsBank));
}

void Soil::AudioSystem::loadBank(std::string bankPath) {
	FMOD::Studio::Bank* bankPtr = NULL;
	this->fmodResult = fmodSystem->loadBankFile(bankPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bankPtr);
	this->fmodBanks[bankPath] = bankPtr;
	this->fmodResult = this->fmodBanks[bankPath]->loadSampleData();
}

void Soil::AudioSystem::loadEventInstance(std::string eventName) {
	FMOD::Studio::EventDescription* eventDescription = NULL;
	auto tempStr = "event:/" + eventName;
	auto tempStrPtr = tempStr.c_str();
	fmodSystem->getEvent(tempStrPtr, &eventDescription);

	FMOD::Studio::EventInstance* eventInstance = NULL;
	eventDescription->createInstance(&eventInstance);

	fmodEventInstances[eventName] = eventInstance;
}

void Soil::AudioSystem::fmodUpdate() {
	fmodResult = fmodSystem->update();
}

void Soil::AudioSystem::startEventInstance(std::string eventName) {
	fmodEventInstances[eventName]->start();
}

void Soil::AudioSystem::stopEventInstance(std::string eventName, bool fadeOut) {
	fmodEventInstances[eventName]->stop(fadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
}