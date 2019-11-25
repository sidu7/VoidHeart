#include <hollowpch.h>
#include "SceneManager.h"
#include "MemoryManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "Hollow/Core/GameObjectFactory.h"
#include "PhysicsManager.h"

#include "Hollow/Components/Transform.h"

namespace Hollow
{
	void SceneManager::Init()
	{
		selectedComponentName = "";
		selectedComponent = nullptr;
		selectedGameObject = nullptr;
	}

	void SceneManager::CleanUp()
	{
	}

	void SceneManager::DebugDisplay()
	{
		if (ImGui::BeginTabBar("Hollow Editor"))
		{			
			if (ImGui::BeginTabItem("Create Prefab"))
			{
				if(ImGui::Button("Create Empty Gameobject"))
				{
					selectedGameObject = MemoryManager::Instance().NewGameObject();
					GameObjectManager::Instance().AddGameObject(selectedGameObject);
					selectedGameObject->AddComponent(MemoryManager::Instance().NewComponent("Transform"));
				}
				if (ImGui::BeginCombo("Components",selectedComponentName.c_str()))
				{
					for (auto& component : MemoryManager::Instance().mComponents)
					{
						bool isSelected = (selectedComponentName == component.first.c_str());
						if (ImGui::Selectable(component.first.c_str(), isSelected))
						{
							if(selectedComponent != nullptr)
							{
								MemoryManager::Instance().DeleteComponent(selectedComponent);
								selectedComponent = nullptr;
							}
							selectedComponentName = component.first;
							selectedComponent = MemoryManager::Instance().NewComponent(selectedComponentName);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if(selectedComponentName != "")
				{
					selectedComponent->DebugDisplay();
				}
				if(ImGui::Button("Add Component"))
				{
					selectedGameObject->AddComponent(selectedComponent);
					selectedComponent = nullptr;
					selectedComponentName = "";
				}
				ImGui::InputText("FileName",charBuffer,255);
				if(ImGui::Button("Export Prefab"))
				{
					DeserializeGameObject();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Create Level"))
			{
				ImGui::Text("Save current scene to Level file");
				ImGui::InputText("Level file name", charBuffer, 255);
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
		std::string contents = ResourceManager::Instance().LoadJSONFile(fileName);
		PARSE_JSON_FILE(contents.c_str());
		
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
	}

	void SceneManager::DeserializeGameObject()
	{
		std::string exportDir = "Resources/Prefabs/";
		exportDir += charBuffer;
		exportDir += ".json";
		std::ofstream file(exportDir.c_str());
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		selectedGameObject->Deserialize(writer);
		file.write(s.GetString(), s.GetSize());
		HW_CORE_INFO("JSON File {0} has been created",exportDir);
	}

	void SceneManager::DeserializeLevel()
	{
		std::string exportDir = "Resources/Levels/";
		exportDir += charBuffer;
		exportDir += ".json";
		std::ofstream file(exportDir.c_str());
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
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
