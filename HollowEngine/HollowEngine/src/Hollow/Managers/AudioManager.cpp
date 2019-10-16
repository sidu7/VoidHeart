#include <hollowpch.h>

#include "AudioManager.h"
#include "ResourceManager.h"

namespace Hollow {
	void AudioManager::Init() 
	{
		FMOD_RESULT result = FMOD_RESULT::FMOD_OK;

		// Create a FMOD Audio system
		result = FMOD::System_Create(&mpSystem);
		if (result != FMOD_RESULT::FMOD_OK)
		{
			HW_CORE_ERROR("Unable to create FMOD audio system");
		}

		// Check version
		unsigned int version = 0;
		result = mpSystem->getVersion(&version);
		if (result != FMOD_RESULT::FMOD_OK)
		{
			HW_CORE_ERROR("FMOD Lib version doesn't match header version");
		}

		// Initialize system with 32 channels
		void* extraDriverData = 0;
		result = mpSystem->init(32, FMOD_INIT_NORMAL, extraDriverData);
		if (result != FMOD_RESULT::FMOD_OK)
		{
			HW_CORE_ERROR("Unable to initialize FMOD audio system");
		}

		// Create channel groups
		mpSystem->getMasterChannelGroup(&mpMasterChannel);
		for (unsigned int channelIndex = 0; channelIndex < SOUND_NUM; ++channelIndex)
		{
			mpSystem->createChannelGroup(0, &mpChannelGroups[channelIndex]);
			mpMasterChannel->addGroup(mpChannelGroups[channelIndex]);
		}

		// Set sound modes for each category
		mModes[SOUND_EFFECT] = FMOD_DEFAULT | FMOD_LOOP_OFF;
		mModes[SOUND_BACKGROUND] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;
		mModes[SOUND_UI] = FMOD_DEFAULT | FMOD_LOOP_OFF;		
	}

	void AudioManager::CleanUp()
	{
		mpSystem->release();
		delete mpSystem;
	}

	void AudioManager::Update()
	{

		if (mpCurrentSong == nullptr)
		{
			PlaySong("Resources/Audio/Songs/test.wav");
		}
		// Update FMOD audio system
		mpSystem->update();
	}

	void AudioManager::PlaySong(const std::string& songName)
	{
		FMOD::Sound* song = ResourceManager::Instance().LoadSound(songName, mModes[SOUND_BACKGROUND]);
		if (song == nullptr)
		{
			HW_CORE_WARN("Unable to load song {0}", songName);
		}
		// Set current song
		//mCurrentSongName = songName;
		mpSystem->playSound(song, 0, true, &mpCurrentSong);
		mpCurrentSong->setChannelGroup(mpChannelGroups[SOUND_BACKGROUND]);
		mpCurrentSong->setVolume(1.0f);
		mpCurrentSong->setPaused(false);
		//mFadeStatus = FADE_IN;
	}

}  // namespace Hollow