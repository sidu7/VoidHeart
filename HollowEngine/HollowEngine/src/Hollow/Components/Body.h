#pragma once
#include "Component.h"

namespace Hollow {
	class Body : public Component
	{
		REGISTERCOMPONENT(Body, "Body");
	public:
		void Init() {}
		void Clear() {}
		void DebugDisplay() {}
		void Serialize(rapidjson::Value::Object data){}
		virtual ~Body() {}

		//Setters
		inline void SetMass(float mass) { mMass = mass; mInverseMass = 1 / mMass; }
		inline void SetPosition(glm::vec3 position) { mPosition = position; }
		inline void SetPreviousPosition(glm::vec3 previousPosition) { mPreviousPosition = previousPosition; }
		inline void SetVelocity(glm::vec3 velocity) { mVelocity = velocity; }
		inline void SetAcceleration(glm::vec3 acceleration) { mAcceleration = acceleration; }
		inline void SetTotalForce(glm::vec3 totalForce) { mTotalForce = totalForce; }
		inline void SetQuaternion(glm::fquat quaternion) { mQuaternion = quaternion; }
		inline void SetRotationMatrix(glm::mat3 rotationMatrix) { mRotationMatrix = rotationMatrix; }
		inline void SetInertiaTensor(glm::mat3 inertiaTensor) { mInertiaTensor = inertiaTensor; mInertiaInverse = glm::inverse(mInertiaTensor); }
		inline void SetWorldSpaceInertia(glm::mat3 worldSpaceInertia) { mWorldSpaceInertia = worldSpaceInertia; }
		inline void SetTorque(glm::vec3 torque) { mTorque = torque; }
		inline void SetAngularVelocity(glm::vec3 angularVelocity) { mAngularVelocity = angularVelocity; }

		//Getters
		inline float GetMass() { return mMass; }
		inline glm::vec3 GetPosition() { return mPosition; }
		inline glm::vec3 GetPreviousPosition() { return mPreviousPosition; }
		inline glm::vec3 GetVelocity() { return mVelocity; }
		inline glm::vec3 GetAcceleration() { return mAcceleration; }
		inline glm::vec3 GetTotalForce() { return mTotalForce; }
		inline glm::fquat GetQuaternion() { return mQuaternion; }
		inline glm::mat3 GetRotationMatrix() { return mRotationMatrix; }
		inline glm::mat3 GetInertiaTensor() { return mInertiaTensor; }
		inline glm::mat3 GetWorldSpaceInertia() { return mWorldSpaceInertia; }
		inline glm::vec3 GetTorque() { return mTorque; }
		inline glm::vec3 GetAngularVelocity() { return mAngularVelocity; }

	private:
		float mMass, mInverseMass;
		glm::vec3 mPosition;
		glm::vec3 mPreviousPosition;
		glm::vec3 mVelocity;
		glm::vec3 mAcceleration;
		glm::vec3 mTotalForce;

		glm::fquat mQuaternion;
		glm::mat3  mRotationMatrix;
		glm::mat3  mInertiaTensor;
		glm::mat3  mInertiaInverse;
		glm::mat3  mWorldSpaceInertia;
		glm::vec3  mTorque;
		glm::vec3  mAngularVelocity;
	};
}