#include "core/core.h"

#include <fmod.hpp>
#include <fmod_errors.h>
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
	FMOD::System *system = nullptr;
	FMOD::Sound *sound = nullptr;
	FMOD::Channel *channel = nullptr;

	FMOD_RESULT result;

	result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		DM_ERROR_LOG("FMOD error: {0}", FMOD_ErrorString(result));
		return -1;
	}

	result = system->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		DM_ERROR_LOG("FMOD init error: {0}", FMOD_ErrorString(result));
		return -1;
	}

	result = system->createSound("wave.mp3", FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK)
	{
		DM_ERROR_LOG("Sound load error: {0}", FMOD_ErrorString(result));
		return -1;
	}

	result = system->playSound(sound, nullptr, false, &channel);
	if (result != FMOD_OK)
	{
		DM_ERROR_LOG("Play error: {0}", FMOD_ErrorString(result));
		return -1;
	}

	// Wait while the sound plays
	bool isPlaying = true;
	while (isPlaying)
	{
		system->update();
		channel->isPlaying(&isPlaying);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	// Clean up
	sound->release();
	system->close();
	system->release();

	return 0;
}
