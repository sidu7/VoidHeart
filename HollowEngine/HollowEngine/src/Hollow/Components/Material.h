#pragma once
#include "Component.h"

namespace Hollow {
	class Texture;
	struct MaterialData;
		
	class Material : public Component
	{
		REGISTERCOMPONENT(Material);
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
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
	};
}
