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
		
		public:
			FMOD::System* mpSystem;

			FMOD::ChannelGroup* mpMasterChannel;
			FMOD::ChannelGroup* mpChannelGroups[SOUND_NUM];

			FMOD_MODE mModes[SOUND_NUM];

			FMOD::Channel* mpCurrentSong;

		private:


	};
}  // namespace Hollow