#pragma once
#include "Component.h"

namespace Hollow
{
	class HOLLOW_API Transform : public Component
	{
		REGISTERCOMPONENT(Transform);
	public:

		void Init();
		void Clear() {}
		void DebugDisplay();
		void Serialize(rapidjson::Value::Object data);

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