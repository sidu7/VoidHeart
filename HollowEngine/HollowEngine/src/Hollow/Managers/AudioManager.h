#pragma once

namespace Hollow {

	enum SOUND_TYPE
	{
		SOUND_BACKGROUND,
		SOUND_EFFECT,
		SOUND_UI,
		SOUND_NUM
	};

	class HOLLOW_API AudioManager 
	{
		SINGLETON(AudioManager)
		public:
			void Init();
			void CleanUp();
			void Update();

			void PlaySong(const std::string& songName);
			void PlayEffect(const std::string& effectName);

			void Mute();
			void MuteChannel(SOUND_TYPE channel);

			void SetVolume();
			void SetChannelVolume(SOUND_TYPE channel);

			void DebugDisplay();
		
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