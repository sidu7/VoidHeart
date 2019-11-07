#pragma once
#include "Component.h"

namespace Hollow
{
	class Transform : public Component
	{
		REGISTERCOMPONENT(Transform);
	public:

		HOLLOW_API void Init();
		HOLLOW_API void Clear() {}
		HOLLOW_API void DebugDisplay();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

		glm::vec3 mPosition;					// mPosition is x,y,z
		glm::vec3 mScale;						// mScale is x,y,z
		glm::vec3 mRotation;					// mRotation is degree for x,y,z
		glm::mat4 mTransformationMatrix;		// Transformation Matrix
		glm::mat4 mDebugTransformationMatrix;	// Debug Transformation Matrix
		glm::mat4 mPreviousTransformationMatrix;// For PostProcessing effects
		glm::fquat mQuaternion;					// mQuaternion Do not mess if you don't know this
		
		bool dirtyBit;

	};
}