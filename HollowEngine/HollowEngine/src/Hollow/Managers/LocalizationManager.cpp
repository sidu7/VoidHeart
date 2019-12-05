#include <hollowpch.h>
#include "LocalizationManager.h"
#include "RenderManager.h"

#include "Hollow/Graphics/TextRenderer.h"

namespace Hollow
{
	void LocalizationManager::Init(rapidjson::Value::Object& data)
	{
		std::string fileDir = data["LanguageFiles"].GetString();
		
		if(std::filesystem::is_directory(fileDir))
		{
			for(std::filesystem::directory_entry entry : std::filesystem::directory_iterator(fileDir))
			{
				std::string fileName = entry.path().filename().replace_extension("").string();
				mLanguageFiles[fileName] = entry.path().string();				
			}
		}
		else
		{
			HW_CORE_ERROR("Language Files Directory not found!!!");
		}
		
		std::string fileName = data["DefaultLanguage"].GetString();
		
		ChangeLanguage(fileName);
	}

	void LocalizationManager::ChangeLanguage(std::string language)
	{
		mCurrentLanguage = language;
		std::string fileName = mLanguageFiles[language];
		
		std::ifstream file(fileName);
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		PARSE_JSON_FILE(contents.c_str());

		RenderManager::Instance().mpTextRenderer->LoadFont(root["FontFile"].GetString());
		for (rapidjson::Value::ConstMemberIterator itr = root.MemberBegin();
			itr != root.MemberEnd(); ++itr)
		{
			mCurrentLanguageMap[itr->name.GetString()] = itr->value.GetString();
		}
	}
}
