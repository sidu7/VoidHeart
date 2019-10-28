#pragma once

namespace Hollow
{

#define MAX_PARTICLES_COUNT 2000000

	class Mesh;
	class VertexArray;
	class VertexBuffer;
	class Texture;
	class ShaderStorageBuffer;

	struct Particle
	{
		glm::vec3 mPosition;
		//glm::vec3 mScale;
		//glm::vec3 mRotation;
		//glm::vec3 mDirection;
		float mSpeed;
		float mLife;
		glm::vec3 mPadding;
		//glm::vec3 mColor;
	};

	enum ParticleType
	{
		POINT,
		MODEL
	};

	struct ParticleData
	{
		std::vector<Mesh*> mParticleModel;
		VertexArray* mpParticleVAO;
		VertexBuffer* mpParticleVBO;
		unsigned int mParticlesCount;
		glm::mat4 mModel;
		Texture* mTex;
		ParticleType mType;
		std::vector<glm::vec4> mParticlePositionList;
		std::vector<glm::mat4> mParticleModelMatrices;
		ShaderStorageBuffer* mpParticleDataStorage;
		ShaderStorageBuffer* mpDeadParticleStorage;
		glm::vec3 mCenter;
	};
}
