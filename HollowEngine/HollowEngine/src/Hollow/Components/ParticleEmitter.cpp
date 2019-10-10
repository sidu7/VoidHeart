#include <hollowpch.h>
#include "ParticleEmitter.h"

namespace Hollow
{
	ParticleEmitter ParticleEmitter::instance;

	void ParticleEmitter::Init()
	{
		mCount = 0;
		mMinSpeed = 0.0f;
		mMaxSpeed = 0.0f;
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
	}

	void ParticleEmitter::Clear()
	{
		mCount = 0;
		mMinSpeed = 0.0f;
		mMaxSpeed = 0.0f;
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
	}

	void ParticleEmitter::Serialize(rapidjson::Value::Object data)
	{
	}

	void ParticleEmitter::DebugDisplay()
	{
	}

}