#pragma once
#include "Hollow/Common.h"

namespace Hollow {

	enum SOUND_TYPE
	{
		SOUND_BACKGROUND,
		SOUND_EFFECT,
		SOUND_UI,
		SOUND_NUM
	};

	class AudioManager 
	{
		SINGLETON(AudioManager)
		public:
			HOLLOW_API void Init();
			HOLLOW_API void CleanUp();
			HOLLOW_API void Update();
			 
			HOLLOW_API void PlaySong(const std::string& songName);
			HOLLOW_API void PlayEffect(const std::string& effectName);
			 
			HOLLOW_API void Mute();
			HOLLOW_API void MuteChannel(SOUND_TYPE channel);
			 
			HOLLOW_API void SetVolume();
			HOLLOW_API void SetChannelVolume(SOUND_TYPE channel);
			 
			HOLLOW_API void DebugDisplay();
		
		public:
			FMOD::System* mpSystem;

			FMOD::ChannelGroup* mpMasterChannel;
			FMOD::ChannelGroup* mpChannelGroups[SOUND_NUM];
			bool mMute[SOUND_NUM];
			float mVolume[SOUND_NUM];
			float mMasterVolume;

			FMOD_MODE mModes[SOUND_NUM];

			FMOD::Channel* mpCurrentSong;
			std::string mCurrentSongPath;

		private:
			void DebugDisplaySongs();
			void DebugDisplaySFX();

	};
}  // namespace Hollow