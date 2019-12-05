#include <hollowpch.h>
#include "LocalizationManager.h"
#include "RenderManager.h"
#include <rapidjson/encodings.h>
#include "Hollow/Graphics/TextRenderer.h"
#include <codecvt>

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
		rapidjson::Document root; 
		root.Parse(contents.c_str());
		
		if (!root.IsObject()) { HW_CORE_ERROR("Error reading JSON file {0}", contents.c_str()); }
	
		RenderManager::Instance().mpTextRenderer->LoadFont(root["FontFile"].GetString());
		for (rapidjson::Value::ConstMemberIterator itr = root.MemberBegin();
			itr != root.MemberEnd(); ++itr)
		{
			//std::string val(itr->value.GetString(), itr->value.GetStringLength());
			mCurrentLanguageMap[itr->name.GetString()] = itr->value.GetString();
		}
	}
}
