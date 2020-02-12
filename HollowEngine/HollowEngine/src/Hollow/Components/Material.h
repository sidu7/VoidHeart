#pragma once
#include "Component.h"

namespace Hollow {
	class Texture;
	struct MaterialData;
		
	class Material : public Component
	{
		REGISTERCOMPONENT(Material);
	public:
		/// <summary>
		/// Initializes the Material component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the Material component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the Material component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current Material component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the Material Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	private:
		std::string mTexturePath;
		std::string mDiffuseTexturePath;
		std::string mSpecularTexturePath;
		std::string mNormalTexturePath;
		std::string mHeightTexturePath;
		std::string mMaterialDataPath;
		bool mHasMaterialTextures;

	public:
		glm::vec3 mDiffuseColor;
		glm::vec3 mSpecularColor;
		float mShininess;
		float mHeightScale;
		Texture* mpTexture;
		std::vector<MaterialData*> mMaterials;
		bool mTransparent;
		float mAlphaValue;
	};
}
