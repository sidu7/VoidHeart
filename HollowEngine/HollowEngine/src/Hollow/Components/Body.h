#pragma once
#include "Component.h"

namespace Hollow {
	class HOLLOW_API Body : public Component
	{
		REGISTERCOMPONENT(Body);
	public:
		void Init();
		void Clear();
		void DebugDisplay();
		void Serialize(rapidjson::Value::Object data);

		float mMass, mInverseMass;
		glm::vec3 mPosition;
		glm::vec3 mPreviousPosition;
		glm::vec3 mVelocity;
		glm::vec3 mAngularVelocity;
		glm::vec3 mTotalForce;
		glm::vec3 mTotalTorque;

		glm::fquat mQuaternion;
		glm::fquat mPreviousQuaternion;
		
		glm::mat3 mRotationMatrix; // TODO dont need to store this... being used in a hack in SAT in place of Transformation matrix
		glm::mat3 mLocalInertiaInverse;
		glm::mat3 mWorldInertiaInverse;
	};
}