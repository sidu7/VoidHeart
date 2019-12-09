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

	/// <summary>
	/// Class Audio Manager.
	/// </summary>
	class AudioManager
	{
		SINGLETON(AudioManager)
		public:
			/// <summary>
			/// Initializes manager data.
			/// </summary>
			HOLLOW_API void Init();
		
			/// <summary>
			/// Cleans manager data.
			/// </summary>
			HOLLOW_API void CleanUp();
		
			/// <summary>
			/// Updates this instance.
			/// </summary>
			HOLLOW_API void Update();
			 
			/// <summary>
			/// Plays the song.
			/// </summary>
			/// <param name="songName">Name of the song.</param>
			HOLLOW_API void PlaySong(const std::string& songName);
		
			/// <summary>
			/// Plays the SFX.
			/// </summary>
			/// <param name="effectName">Name of the SFX.</param>
			HOLLOW_API void PlayEffect(const std::string& effectName);
			 
			/// <summary>
			/// Mutes all audio channels.
			/// </summary>
			HOLLOW_API void Mute();
		
			/// <summary>
			/// Mutes the channel.
			/// </summary>
			/// <param name="channel">The channel type.</param>
			HOLLOW_API void MuteChannel(SOUND_TYPE channel);
			 
			/// <summary>
			/// Sets the volume.
			/// </summary>
			HOLLOW_API void SetVolume();
		
			/// <summary>
			/// Sets the channel volume.
			/// </summary>
			/// <param name="channel">The channel type.</param>
			HOLLOW_API void SetChannelVolume(SOUND_TYPE channel);
			 
			/// <summary>
			/// Displays debug window.
			/// </summary>
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