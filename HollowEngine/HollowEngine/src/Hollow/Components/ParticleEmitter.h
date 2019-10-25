#pragma once
#include "Component.h"

namespace Hollow
{
	struct ParticleData;
	class Mesh;
	class VertexBuffer;
	class VertexArray;
	class Texture;
	enum ParticleType;
	class ShaderStorageBuffer;

	struct Particle
	{
		glm::vec3 mPosition;
		glm::vec3 mScale;
		glm::vec3 mRotation;
		glm::vec3 mDirection;
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

	public:
		unsigned long mCount;
		std::vector<Particle> mParticlesList;

		// For Point particles
		std::vector<glm::vec4> mParticlePositions;
		VertexBuffer* mpParticlePositionVBO;
		VertexArray* mpParticlePositionVAO;
		Texture* mTexture;

		// For Mesh particles
		std::vector<Mesh*> mpParticle;
		std::vector<glm::mat4> mModelMatrices;
		VertexBuffer* mpParticleModelVBO;

		ParticleType mType;
		float mMinSpeed;
		float mMaxSpeed;
		float mMinLife;
		float mMaxLife;
		glm::vec3 mCenterOffset;
		glm::vec3 mAreaOfEffect;
		//TODO: direction data and scales

		//Test
		ShaderStorageBuffer* mShaderStorage;
	};
}