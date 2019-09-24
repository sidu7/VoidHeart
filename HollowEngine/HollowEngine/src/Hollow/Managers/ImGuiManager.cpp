#include <hollowpch.h>
#include "ImGuiManager.h"

#include "Hollow/Graphics/GameWindow.h"

#include "GameObjectManager.h"
#include "Hollow/Core/GameObject.h"

namespace Hollow {

	void ImGuiManager::Init(GameWindow* pWindow)
	{
		// Create reference to window
		mpWindow = pWindow;
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
		if (ImGui::Begin("Game Objects"))
		{
			for (GameObject* pGameObject : GameObjectManager::Instance().GetGameObjects())
			{
				pGameObject->DebugDisplay();
			}
		}
		ImGui::End();

		// Show demo window for now
		//ImGui::ShowDemoWindow((bool*)1);

		Render();
	}

	void ImGuiManager::StartFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(mpWindow->GetWindow());
		ImGui::NewFrame();
	}

	void ImGuiManager::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}