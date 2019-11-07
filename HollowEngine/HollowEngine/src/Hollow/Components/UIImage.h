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
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		HOLLOW_API void DebugDisplay();

	public:
		Texture* mpTexture;
		glm::vec3 mColor;
		Mesh* mpShapeData;
		Shapes mShapeType;
	};
}