#include <hollowpch.h>
#include "Material.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Texture.h"

namespace Hollow {

	Material Material::instance;

	void Material::Init()
	{
		mpTexture = nullptr;
		mDiffuseColor = COLOR_BLACK;
		mSpecularColor = COLOR_BLACK;
		mShininess = 0.0f;
	}

	void Material::Clear()
	{
		delete mpTexture;
		mMaterials.clear();
		mDiffuseColor = COLOR_BLACK;
		mSpecularColor = COLOR_BLACK;
		mShininess = 0.0f;
	}

	void Material::Serialize(rapidjson::Value::Object data)
	{		
		if (data.HasMember("Texture"))
		{
			std::string path = data["Texture"].GetString();
			mpTexture = ResourceManager::Instance().LoadTexture(path);
		}
		if (data.HasMember("Diffuse"))
		{
			mDiffuseColor = JSONHelper::GetVec3F(data["Diffuse"].GetArray());
		}
		if (data.HasMember("Specular"))
		{
			mSpecularColor = JSONHelper::GetVec3F(data["Specular"].GetArray());
		}
		if (data.HasMember("Shininess"))
		{
			mShininess = data["Shininess"].GetFloat();
		}
		if (data.HasMember("MaterialTextures"))
		{
			MaterialData* materials = new MaterialData();
			if (data.HasMember("DiffuseTexture"))
			{
				std::string path = data["DiffuseTexture"].GetString();
				materials->mpDiffuse = ResourceManager::Instance().LoadTexture(path);
			}
			if (data.HasMember("SpecularTexture"))
			{
				std::string path = data["SpecularTexture"].GetString();
				materials->mpSpecular = ResourceManager::Instance().LoadTexture(path);
			}
			if (data.HasMember("NormalTexture"))
			{
				std::string path = data["NormalTexture"].GetString();
				materials->mpNormal = ResourceManager::Instance().LoadTexture(path);
			}
			if (data.HasMember("HeightTexture"))
			{
				std::string path = data["HeightTexture"].GetString();
				materials->mpHeight = ResourceManager::Instance().LoadTexture(path);
			}
			if (data.HasMember("HeightScale"))
			{
				mHeightScale = data["HeightScale"].GetFloat();
			}

			mMaterials.push_back(materials);
		}
		if (data.HasMember("MaterialData"))
		{
			std::string path = data["MaterialData"].GetString();
			mMaterials = ResourceManager::Instance().LoadMaterials(path);
		}
	}

	void Material::DebugDisplay()
	{
		if (ImGui::TreeNode("Material"))
		{
			// Show the diffuse color
			ImGui::ColorEdit3("Diffuse", &mDiffuseColor[0], ImGuiColorEditFlags_Float);

			// Show the specular color
			ImGui::ColorEdit3("Specular", &mSpecularColor[0], ImGuiColorEditFlags_Float);

			// Show the shininess
			ImGui::InputFloat("Shininess", &mShininess);

			// Show parallax map height scale
			ImGui::InputFloat("Height Scale", &mHeightScale);

			// Show the texture if there is only one
			if (mpTexture)
			{
				ImGui::Image((void*)mpTexture->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			}
			 // Show a small version of the currently selected texture
			for (MaterialData* material : mMaterials)
			{
				if (material->mpDiffuse)
				{
					ImGui::Text("Diffuse texture");
					ImGui::Image((void*)material->mpDiffuse->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
				}
				if (material->mpSpecular)
				{
					ImGui::Text("Specular texture");
					ImGui::Image((void*)material->mpSpecular->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
				}
				if (material->mpNormal)
				{
					ImGui::Text("Normal texture");
					ImGui::Image((void*)material->mpNormal->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
				}
				if (material->mpHeight)
				{
					ImGui::Text("Height texture");
					ImGui::Image((void*)material->mpHeight->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
				}
			}

			ImGui::TreePop();
		}
	}
}