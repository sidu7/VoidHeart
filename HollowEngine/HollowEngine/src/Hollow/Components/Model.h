#pragma once
#include "Component.h"

namespace Hollow
{
	class Mesh;
	class Shader;

	class Model : public Component
	{
		REGISTERCOMPONENT(Model);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object object);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		HOLLOW_API void DebugDisplay();

		std::vector<Mesh*> mMeshes;
		Shader* mpShader;
		bool mCastShadow;
		bool mModelHasOffset;

		// variables for deserialization... can be private
		unsigned int mShape;
		std::string mModelPath;
	};
}