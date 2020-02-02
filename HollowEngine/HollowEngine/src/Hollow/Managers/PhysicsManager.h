#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Physics/Broadphase/DynamicAABBTree.h"
#include "Hollow/Physics/NarrowPhase/SAT.h"

namespace Hollow
{
	class Camera;
	class Collider;
	
	/// <summary>
	/// Class Physics Manager.
	/// </summary>
	class PhysicsManager
	{
		SINGLETON(PhysicsManager)
	public:
    
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API void CleanUp();
    
		/// <summary>
		/// Applies the angular impulse.
		/// </summary>
		/// <param name="object">The game object.</param>
		/// <param name="impulse">The impulse value.</param>
		HOLLOW_API  void ApplyAngularImpulse(GameObject* object, glm::vec3 impulse);
		
		/// <summary>
		/// Applies the linear impulse.
		/// </summary>
		/// <param name="object">The game object.</param>
		/// <param name="impulse">The impulse value.</param>
		HOLLOW_API  void ApplyLinearImpulse(GameObject* object, glm::vec3 impulse);

		/// <summary>
		/// Returns nearest game object in the direction of the ray.
		/// </summary>
		/// <returns>Hollow.GameObject *.</returns>
		HOLLOW_API  GameObject* CastRay();

		/// <summary>
		/// Initializes manager using settings data.
		/// </summary>
		/// <param name="data">The settings data.</param>
		HOLLOW_API void Init(rapidjson::Value::Object& data);

		HOLLOW_API void UpdateScale(GameObject* pGo);
	public:
		// Data
		DynamicAABBTree mTree;
		// SAT
		SAT mSAT;		
		std::unordered_map<std::string, Body::RigidbodyType> mRigidbodyTypesMap;
		std::unordered_map<unsigned int, bool > mCollisionMask;
	};
}
