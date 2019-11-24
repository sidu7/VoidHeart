#pragma once
#include "Component.h"

namespace Hollow
{
	class Texture;
	class Mesh;
	enum Shapes;

	class UIImage : public Component
	{
		REGISTERCOMPONENT(UIImage);
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		HOLLOW_API void DebugDisplay() override;
		
	private:
		std::string TexturePath;
		std::string mShapeType;
		
	public:
		Texture* mpTexture;
		glm::vec3 mColor;
		Mesh* mpShapeData;
	};
}