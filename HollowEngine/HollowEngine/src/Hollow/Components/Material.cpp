#include <hollowpch.h>
#include "Material.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Data/Color.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Utils/ImGuiHelper.h"

//#pragma warning (disable : 4312)

namespace Hollow {

	Material Material::instance;

	void Material::Init()
	{
		mDiffuseColor = COLOR_BLACK;
		mSpecularColor = COLOR_BLACK;
		mShininess = 0.0f;
		mHeightScale = 0.0f;
		mpTexture = nullptr;

		mTexturePath = "";
		mDiffuseTexturePath = "";
		mSpecularTexturePath = "";
		mNormalTexturePath = "";
		mHeightTexturePath = "";
		mMaterialDataPath = "";
		mHasMaterialTextures = false;
		mTransparent = false;
		mAlphaValue = 1.0f;
	}

	void Material::Clear()
	{
		mMaterials.clear();
	}

	void Material::Serialize(rapidjson::Value::Object data)
	{		
		if (data.HasMember("Texture"))
		{
			mTexturePath = data["Texture"].GetString();
			mpTexture = ResourceManager::Instance().LoadTexture(mTexturePath);
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
			mHasMaterialTextures = true;
			MaterialData* materials = new MaterialData();
			if (data.HasMember("DiffuseTexture"))
			{
				mDiffuseTexturePath = data["DiffuseTexture"].GetString();
				materials->mpDiffuse = ResourceManager::Instance().LoadTexture(mDiffuseTexturePath);
			}
			if (data.HasMember("SpecularTexture"))
			{
				mSpecularTexturePath = data["SpecularTexture"].GetString();
				materials->mpSpecular = ResourceManager::Instance().LoadTexture(mSpecularTexturePath);
			}
			if (data.HasMember("NormalTexture"))
			{
				mNormalTexturePath = data["NormalTexture"].GetString();
				materials->mpNormal = ResourceManager::Instance().LoadTexture(mNormalTexturePath);
			}
			if (data.HasMember("HeightTexture"))
			{
				mHeightTexturePath = data["HeightTexture"].GetString();
				materials->mpHeight = ResourceManager::Instance().LoadTexture(mHeightTexturePath);
			}
			if (data.HasMember("HeightScale"))
			{
				mHeightScale = data["HeightScale"].GetFloat();
			}

			mMaterials.push_back(materials);
		}
		if (data.HasMember("MaterialData"))
		{
			mMaterialDataPath = data["MaterialData"].GetString();
			mMaterials = ResourceManager::Instance().LoadMaterials(mMaterialDataPath);
		}
		if (data.HasMember("Transparent"))
		{
			mTransparent = data["Transparent"].GetBool();
		}
		if (data.HasMember("AlphaValue"))
		{
			mAlphaValue = data["AlphaValue"].GetFloat();
		}
	}

	void Material::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Texture", mTexturePath, writer);
		JSONHelper::Write("Diffuse", mDiffuseColor, writer);
		JSONHelper::Write("Specular", mSpecularColor, writer);
		JSONHelper::Write("Shininess", mShininess, writer);
		JSONHelper::Write("MaterialTextures", mHasMaterialTextures, writer);
		JSONHelper::Write("DiffuseTexture", mDiffuseTexturePath, writer);
		JSONHelper::Write("SpecularTexture", mSpecularTexturePath, writer);
		JSONHelper::Write("NormalTexture", mNormalTexturePath, writer);
		JSONHelper::Write("HeightTexture", mHeightTexturePath, writer);
		JSONHelper::Write("HeightScale", mHeightScale, writer);
		JSONHelper::Write("MaterialData", mMaterialDataPath, writer);
		JSONHelper::Write("Transparent", mTransparent, writer);
		JSONHelper::Write("AlphaValue", mAlphaValue, writer);
	}

	void Material::DebugDisplay()
	{
		// Show the diffuse color
		ImGui::ColorEdit3("Diffuse", &mDiffuseColor[0], ImGuiColorEditFlags_Float);

		// Show the specular color
		ImGui::ColorEdit3("Specular", &mSpecularColor[0], ImGuiColorEditFlags_Float);

		// Show the shininess
		ImGui::InputFloat("Shininess", &mShininess);

		ImGui::Checkbox("Transparent", &mTransparent);
		ImGui::InputFloat("Alpha Value", &mAlphaValue);

		ImGuiHelper::InputText("Texture File", mTexturePath);

		// Show the texture if there is only one
		if (mpTexture)
		{
			ImGui::Image((void*)(intptr_t)(mpTexture->GetTextureID()), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
		}

		ImGui::Checkbox("Has Material Textures", &mHasMaterialTextures);
		if (mHasMaterialTextures)
		{
			ImGuiHelper::InputText("Diffuse Texture Path", mDiffuseTexturePath);
			ImGuiHelper::InputText("Specular Texture Path", mSpecularTexturePath);
			ImGuiHelper::InputText("Normal Texture Path", mNormalTexturePath);
			ImGuiHelper::InputText("Height Texture Path", mHeightTexturePath);
			// Show parallax map height scale
			ImGui::InputFloat("Height Scale", &mHeightScale);			
		}
		else
		{
			ImGuiHelper::InputText("Material Data File", mMaterialDataPath);
		}
		// Show a small version of the currently selected texture
		for (MaterialData* material : mMaterials)
		{
			if (material->mpDiffuse)
			{
				ImGui::Text("Diffuse texture");
				ImGui::Image((void*)(intptr_t)(material->mpDiffuse->GetTextureID()), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			}
			if (material->mpSpecular)
			{
				ImGui::Text("Specular texture");
				ImGui::Image((void*)(intptr_t)(material->mpSpecular->GetTextureID()), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			}
			if (material->mpNormal)
			{
				ImGui::Text("Normal texture");
				ImGui::Image((void*)(intptr_t)(material->mpNormal->GetTextureID()), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			}
			if (material->mpHeight)
			{
				ImGui::Text("Height texture");
				ImGui::Image((void*)(intptr_t)(material->mpHeight->GetTextureID()), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			}
		}
	}
}