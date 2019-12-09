#pragma once
#include "Component.h"

namespace Hollow {
	enum RigidbodyType;

	class Body : public Component
	{

		REGISTERCOMPONENT(Body);
	public:
		enum RigidbodyType
		{
#define RIGIDBODY_TYPE(name) name,
#include "Hollow/Enums/RigidbodyTypes.enum"
#undef RIGIDBODY_TYPE
			NUM
		};
		
		/// <summary>
		/// Initializes the Body component.
		/// </summary>
		HOLLOW_API void Init();

		/// <summary>
		/// Clears the Body component.
		/// </summary>
		HOLLOW_API void Clear();

		/// <summary>
		/// To set the Debug Display for the Body Component.
		/// </summary>
		HOLLOW_API void DebugDisplay();

		/// <summary>
		/// Serializes the specified data of the Body component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// DeSerialize the current Body component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

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

		RigidbodyType mBodyType;

		bool mIsFrictionLess;
		bool mUseGravity;
	private:
		std::string mDRigidbodyType;
	};
}
