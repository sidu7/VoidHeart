#pragma once
#include "Component.h"

namespace Hollow
{
	class Transform : public Component
	{
		REGISTERCOMPONENT(Transform);
	public:

		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override {}
		HOLLOW_API void DebugDisplay() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

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