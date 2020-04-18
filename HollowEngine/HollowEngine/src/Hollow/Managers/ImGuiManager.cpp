#include <hollowpch.h>
#include "ImGuiManager.h"

#include "Hollow/Graphics/GameWindow.h"

#include "GameObjectManager.h"
#include "FrameRateController.h"

#include "Hollow/Core/GameObject.h"

#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/AudioManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/LocalizationManager.h"

#include "Hollow/Components/Transform.h"
#include "SceneManager.h"


namespace Hollow {

	void ImGuiManager::Init(GameWindow* pWindow)
	{
		// Create reference to window
		mpWindow = pWindow;

		// Set game object selected to null
		mpSelectedGameObject = nullptr;
		mSelectedGameObjectID = 0;
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		// Set up renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(mpWindow->GetWindow(), mpWindow->GetContext());
		ImGui_ImplOpenGL3_Init("#version 430");
	}

	void ImGuiManager::CleanUp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiManager::Update()
	{
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			ImGui::Begin("Game Objects", NULL, ImGuiWindowFlags_NoScrollWithMouse);
		}
		else
		{
			ImGui::Begin("Game Objects");
		}

		ImGui::Text("FPS: %.2f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("Number Of Active Objects: %i", GameObjectManager::Instance().GetGameObjects().size());
		if(ImGui::Button("Delete GameObject"))
		{
			GameObjectManager::Instance().DeleteGameObject(mpSelectedGameObject);
		}
		// Get selected object 
		ImGui::BeginChild("Left Side", ImVec2(150, 0), true);
		for (GameObject* pGameObject : GameObjectManager::Instance().GetGameObjects())
		{
			unsigned int ID = pGameObject->mID;
			std::string name = "Object " + std::to_string(ID);
			if (ImGui::Selectable(name.c_str(), ID == mSelectedGameObjectID))
			{
				mpSelectedGameObject = pGameObject;
				mSelectedGameObjectID = ID;
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();

		// Display game object information
		ImGui::BeginGroup();
		ImGui::BeginChild("Item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		ImGui::Text("Object: %d", mSelectedGameObjectID);
		ImGui::Separator();
		if (mpSelectedGameObject)
		{
			mpSelectedGameObject->DebugDisplay();
		}
		ImGui::EndChild();
		ImGui::EndGroup();

		ImGui::End();

		// Tabs for larger sytems
		ImGui::Begin("Managers");
		if (ImGui::BeginTabBar("Manager Tab Bar"))
		{
			for (auto display : mDisplayFunctions)
			{
				if (ImGui::BeginTabItem(display.first.c_str()))
				{
					display.second();
					ImGui::EndTabItem();
				}
			}
			if(ImGui::BeginTabItem("Localization"))
			{
				if(ImGui::Button("English"))
				{
					LocalizationManager::Instance().ChangeLanguage("English");
				}
				if(ImGui::Button("Hindi"))
				{
					LocalizationManager::Instance().ChangeLanguage("Hindi");
				}
				if (ImGui::Button("Japanese"))
				{
					LocalizationManager::Instance().ChangeLanguage("Japanese");
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Testing"))
			{
				TestingDisplay();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		// Show demo window for now
		//ImGui::ShowDemoWindow();

		Render();
	}

	void ImGuiManager::StartFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(mpWindow->GetWindow());
		ImGui::NewFrame();
	}

	void ImGuiManager::AddDisplayFunction(const std::string& name, std::function<void()> function)
	{
		mDisplayFunctions.push_back(std::make_pair(name, function));
	}

	void ImGuiManager::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiManager::TestingDisplay()
	{
		if (ImGui::Button("Drop Sphere"))
		{

			Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Sphere.json");
		}
		if (ImGui::Button("Create Object"))
		{
			auto& lua = ScriptingManager::Instance().lua;
			lua.script_file("Resources/Scripts/CreateObject.lua");
			std::string path = lua["gameObjectPath"];
			glm::vec3 tra = lua.get<glm::vec3>("position");
			GameObject* pGameObject = Hollow::ResourceManager::Instance().LoadGameObjectFromFile(path);
			pGameObject->GetComponent<Transform>()->mPosition = tra;
		}
	}
}
