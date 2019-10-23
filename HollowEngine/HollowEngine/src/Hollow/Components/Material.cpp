#include <hollowpch.h>
#include "Material.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/RenderData.h"

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

			// Show the texture ID value
			//ImGui::texture
			 // Show a small version of the currently selected texture
			for (MaterialData* material : mMaterials)
			{
				if (material->mpDiffuse)
				{
					ImGui::Image((void*)material->mpDiffuse->GetRendererID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()));
				}
				if (material->mpSpecular)
				{
					ImGui::Image((void*)material->mpSpecular->GetRendererID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()));
				}
				if (material->mpNormal)
				{
					ImGui::Image((void*)material->mpNormal->GetRendererID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()));
				}
				if (material->mpHeight)
				{
					ImGui::Image((void*)material->mpHeight->GetRendererID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()));
				}
			}
			
			//ImGui::InputScalar("Texture", ImGuiDataType_U32, &mTexture);

			ImGui::TreePop();
		}
	}
}