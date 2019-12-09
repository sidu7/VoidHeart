#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	/// <summary>
	/// Class Localization Manager.
	/// </summary>
	class LocalizationManager
	{
		SINGLETON(LocalizationManager);
	public:
		/// <summary>
		/// Initializes manager using settings data.
		/// </summary>
		/// <param name="data">The settings data.</param>
		HOLLOW_API void Init(rapidjson::Value::Object& data);
		/// <summary>
		/// Changes the language.
		/// </summary>
		/// <param name="language">The language.</param>
		HOLLOW_API void ChangeLanguage(std::string language);

	public:
		std::string mCurrentLanguage;
		std::unordered_map<std::string, std::string> mCurrentLanguageMap;
		std::unordered_map<std::string, std::string> mLanguageFiles;
	};
}