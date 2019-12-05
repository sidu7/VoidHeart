#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class LocalizationManager
	{
		SINGLETON(LocalizationManager);
	public:
		HOLLOW_API void Init(rapidjson::Value::Object& data);
		HOLLOW_API void ChangeLanguage(std::string language);

	public:
		std::string mCurrentLanguage;
		std::unordered_map<std::string, std::string> mCurrentLanguageMap;
		std::unordered_map<std::string, std::string> mLanguageFiles;
	};
}