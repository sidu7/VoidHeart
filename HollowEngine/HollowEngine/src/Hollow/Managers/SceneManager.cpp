#include <hollowpch.h>
#include "SystemManager.h"
#include "SceneManager.h"
#include "MemoryManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "Hollow/Core/GameObjectFactory.h"
#include "PhysicsManager.h"
#include "AudioManager.h"

#include "Hollow/Components/Transform.h"
#include "ImGuiManager.h"

namespace Hollow
{
	void SceneManager::Init()
	{
		mSelectedComponentName = "";
		mSelectedComponent = nullptr;
		mSelectedGameObject = nullptr;

		// Add to ImGui debug display
		ImGuiManager::Instance().AddDisplayFunction("Scene", std::bind(&SceneManager::DebugDisplay, &SceneManager::Instance()));
	}

	void SceneManager::CleanUp()
	{
		if (mSelectedGameObject != nullptr)
		{
			MemoryManager::Instance().DeleteGameObject(mSelectedGameObject);
		}
		if (mSelectedComponent != nullptr)
		{
			MemoryManager::Instance().DeleteComponent(mSelectedComponent);
		}
	}

	void SceneManager::DebugDisplay()
	{
		if (ImGui::BeginTabBar("Hollow Editor"))
		{			
			if (ImGui::BeginTabItem("Create Prefab"))
			{
				if(ImGui::Button("Create Empty Gameobject"))
				{
					if(mSelectedGameObject != nullptr)
					{
						MemoryManager::Instance().DeleteGameObject(mSelectedGameObject);
						mSelectedGameObject = nullptr;
					}
					mSelectedGameObject = MemoryManager::Instance().NewGameObject();
					GameObjectManager::Instance().AddGameObject(mSelectedGameObject);
					mSelectedGameObject->AddComponent(MemoryManager::Instance().NewComponent("Transform"));
					ImGuiManager::Instance().mpSelectedGameObject = mSelectedGameObject;
					ImGuiManager::Instance().mSelectedGameObjectID = mSelectedGameObject->mID;
				}
				if (ImGui::BeginCombo("Components",mSelectedComponentName.c_str()))
				{
					for (auto& component : MemoryManager::Instance().mComponents)
					{
						bool isSelected = (mSelectedComponentName == component.first.c_str());
						if (ImGui::Selectable(component.first.c_str(), isSelected))
						{
							if(mSelectedComponent != nullptr)
							{
								MemoryManager::Instance().DeleteComponent(mSelectedComponent);
								mSelectedComponent = nullptr;
							}
							mSelectedComponentName = component.first;
							mSelectedComponent = MemoryManager::Instance().NewComponent(mSelectedComponentName);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if(mSelectedComponentName != "")
				{
					if(ImGui::TreeNode((mSelectedComponentName+" data").c_str()))
					{
						mSelectedComponent->DebugDisplay();
						ImGui::TreePop();
					}
				}
				if(ImGui::Button("Add Component"))
				{
					rapidjson::StringBuffer s;
					rapidjson::Writer<rapidjson::StringBuffer> writer(s);
					writer.StartObject();
					writer.Key("Type");
					writer.String(mSelectedComponentName.c_str());
					mSelectedComponent->DeSerialize(writer);
					writer.EndObject();
					rapidjson::Document comp;
					comp.Parse(s.GetString());
					mSelectedComponent->Serialize(comp.GetObject());
					mSelectedGameObject = ImGuiManager::Instance().mpSelectedGameObject;
					mSelectedGameObject->AddComponent(mSelectedComponent);
					mSelectedComponent = nullptr;
					mSelectedComponentName = "";
					GameObjectManager::Instance().AddGameObject(mSelectedGameObject);
				}
				ImGui::InputText("FileName",charBuffer,255);
				if(ImGui::Button("Export Prefab"))
				{
					DeserializeGameObject();
					mSelectedGameObject = nullptr;
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Load Prefab"))
			{
				ImGui::InputText("Prefab File", charBuffer, 255);
				if (ImGui::Button("Add GameObject"))
				{
					std::string fileName = "Resources/Prefabs/" + std::string(charBuffer) + ".json";
					ResourceManager::Instance().LoadGameObjectFromFile(fileName);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Create Level"))
			{
				ImGui::Text("Save current scene to Level file");
				ImGui::InputText("Level file name", charBuffer, 255);
				if(ImGui::TreeNode("Audio"))
				{
					ImGui::SliderFloat("Master Volume", &AudioManager::Instance().mMasterVolume, 0.0f, 1.0f);
					ImGui::SliderFloat("Music Volume", &AudioManager::Instance().mVolume[SOUND_BACKGROUND], 0.0f, 1.0f);
					ImGui::SliderFloat("SFX Volume", &AudioManager::Instance().mVolume[SOUND_EFFECT],0.0f,1.0f);
					ImGuiHelper::InputText("Background Music", mBackgroundAudio);
					ImGui::TreePop();
				}
				if(ImGui::Button("SaveToFile"))
				{
					DeserializeLevel();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Load Level"))
			{
				ImGui::InputText("Level file name", charBuffer, 255);
				if(ImGui::Button("Load Level"))
				{
					LoadLevel(charBuffer);
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	void SceneManager::LoadLevel(std::string LevelFile)
	{
		GameObjectManager::Instance().DeleteAllGameObjects();
		
		std::string fileName = "Resources/Levels/" + LevelFile + ".json";
		std::ifstream file(fileName);
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		PARSE_JSON_FILE(contents.c_str());

		if(root.HasMember("Audio"))
		{
			auto audio = root["Audio"].GetObject();
			if(audio.HasMember("Volumes"))
			{
				auto volumes = audio["Volumes"].GetObject();
				AudioManager::Instance().mMasterVolume = volumes["Master"].GetFloat();
				AudioManager::Instance().mVolume[SOUND_BACKGROUND] = volumes["Music"].GetFloat();
				AudioManager::Instance().mVolume[SOUND_EFFECT] = volumes["SFX"].GetFloat();
			}
			if (audio.HasMember("Background"))
			{
				std::string path = audio["Background"].GetString();
				AudioManager::Instance().PlaySong(path);
			}
		}
		
		rapidjson::Value::Array gameobjects = root["GameObjects"].GetArray();
		for (unsigned int i = 0; i < gameobjects.Size(); ++i)
		{
			GameObject* pNewGameObject = GameObjectFactory::Instance().LoadObject(gameobjects[i].GetObject());

			if (pNewGameObject)
			{
				GameObjectManager::Instance().AddGameObject(pNewGameObject);
			}
		}
		HW_CORE_INFO("Scene {0} has been loaded", LevelFile);
		//SystemManager::Instance().OnSceneInit();
	}

	void SceneManager::DeserializeGameObject()
	{
		std::string exportDir = "Resources/Prefabs/";
		exportDir += charBuffer;
		exportDir += ".json";
		std::ofstream file(exportDir.c_str());
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		mSelectedGameObject->Deserialize(writer);
		file.write(s.GetString(), s.GetSize());
		HW_CORE_INFO("JSON File {0} has been created",exportDir);
	}

	void SceneManager::DeserializeLevel()
	{
		std::string exportDir = "Resources/Levels/";
		exportDir += charBuffer;
		exportDir += ".json";
		std::ofstream file(exportDir.c_str(),std::ofstream::trunc);
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
		// Audio Deserialize
		writer.Key("Audio");
		writer.StartObject();
		writer.Key("Volumes");
		writer.StartObject();
		writer.Key("Master");
		writer.Double(AudioManager::Instance().mMasterVolume);
		writer.Key("Music");
		writer.Double(AudioManager::Instance().mVolume[SOUND_BACKGROUND]);
		writer.Key("SFX");
		writer.Double(AudioManager::Instance().mVolume[SOUND_EFFECT]);
		writer.EndObject();
		writer.Key("Background");
		writer.String(mBackgroundAudio.c_str());
		writer.EndObject();
		// GameObjects Deserialize
		writer.Key("GameObjects");
		writer.StartArray();
		std::vector<Hollow::GameObject*> gos = Hollow::GameObjectManager::Instance().GetGameObjects();
		for (Hollow::GameObject* g : gos)
		{
			g->Deserialize(writer);
		}
		writer.EndArray();
		writer.EndObject();
		file.write(s.GetString(), s.GetSize());
		HW_CORE_INFO("JSON File {0} has been created", exportDir);
	}
}
