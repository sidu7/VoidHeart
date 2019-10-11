#pragma once
#include "Component.h"

namespace Hollow
{
	struct ParticleData;
	class Mesh;
	class VertexBuffer;

	struct Particle
	{
		glm::vec3 mPosition;
		glm::vec3 mScale;
		glm::vec3 mRotation;
		float mSpeed;
		float mLife;
		glm::vec3 mColor;
	};

	class HOLLOW_API ParticleEmitter : public Component
	{
		REGISTERCOMPONENT(ParticleEmitter)
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DebugDisplay();

	private:
		void UpdateAttributes();

	public:
		unsigned long mCount;
		std::vector<Mesh*> mpParticle;
		std::vector<glm::mat4> mModelMatrices;
		std::vector<Particle> mParticlesList;
		VertexBuffer* mpModelMatricesVBO;
		float mMinSpeed;
		float mMaxSpeed;
		glm::vec3 mCenterOffset;
		glm::vec3 mAreaOfEffect;
		//TODO: direction data and scales
	};
}