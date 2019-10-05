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

		inline void SetPosition(glm::vec3 position) { mPosition = position; }
		inline void SetScale(glm::vec3 scale) { mScale = scale; }
		inline void SetRotation(glm::vec3 rotation) { mRotation = rotation; }
		inline void SetTranformationMatrix(glm::mat4 transformationMatrix) { mTransformationMatrix = transformationMatrix; }
		inline void SetDebugTransformationMatrix(glm::mat4 debugTransformationMatrix) { mDebugTransformationMatrix = debugTransformationMatrix; }
		inline void SetPreviousTransformationMatrix(glm::mat4 previousTransFormationMatrix) { mPreviousTransformationMatrix= previousTransFormationMatrix; }
		inline void SetQuaternion(glm::fquat quaternion) { mQuaternion = quaternion; }


		inline glm::vec3 GetPosition() { return mPosition; }
		inline glm::vec3 GetScale() { return mScale; }
		inline glm::vec3 GetRotation() { return mRotation; }
		inline glm::mat4 GetTranformationMatrix() { return mTransformationMatrix; }
		inline glm::mat4 GetDebugTransformationMatrix() { return mDebugTransformationMatrix; }
		inline glm::mat4 GetPreviousTransformationMatrix() { return mPreviousTransformationMatrix; }
		inline glm::fquat GetQuaternion() { return mQuaternion; }

	private:
		glm::vec3 mPosition;					// mPosition is x,y,z
		glm::vec3 mScale;						// mScale is x,y,z
		glm::vec3 mRotation;					// mRotation is degree for x,y,z
		glm::mat4 mTransformationMatrix;		// Transformation Matrix
		glm::mat4 mDebugTransformationMatrix;	// Debug Transformation Matrix
		glm::mat4 mPreviousTransformationMatrix;// For PostProcessing effects
		glm::fquat mQuaternion;					// mQuaternion Do not mess if you don't know this
	};
}