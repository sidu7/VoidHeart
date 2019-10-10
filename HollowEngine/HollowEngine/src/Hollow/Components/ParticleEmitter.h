#pragma once
#include "Component.h"

namespace Hollow
{
	struct ParticleData;
	class Mesh;

	class HOLLOW_API ParticleEmitter : public Component
	{
		REGISTERCOMPONENT(ParticleEmitter)
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DebugDisplay();

	public:
		unsigned long mCount;
		std::vector<Mesh*> mpParticle;
		float mMinSpeed;
		float mMaxSpeed;
		glm::vec3 mCenterOffset;
		glm::vec3 mAreaOfEffect;
	};
}