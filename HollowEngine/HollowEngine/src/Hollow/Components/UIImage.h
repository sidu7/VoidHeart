#pragma once
#include "Component.h"

namespace Hollow
{
	class Texture;
	class Mesh;
	enum Shapes;

	class HOLLOW_API UIImage : public Component
	{
		REGISTERCOMPONENT(UIImage);
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void DebugDisplay();

	public:
		Texture* mpTexture;
		glm::vec3 mColor;
		Mesh* mpShapeData;
		Shapes mShapeType;
	};
}