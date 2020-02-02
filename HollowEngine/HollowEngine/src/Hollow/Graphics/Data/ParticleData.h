#pragma once

namespace Hollow
{

#define MAX_PARTICLES_COUNT 2000000

	class Mesh;
	class VertexArray;
	class VertexBuffer;
	class Texture;
	class ShaderStorageBuffer;
	struct MaterialData;

	struct Particle
	{
		glm::vec3 mPosition;
		//glm::vec3 mScale;
		//glm::vec3 mRotation;
		//glm::vec3 mDirection;
		float mSpeed;
		float mLife;
		float mCurrentLife;
		glm::vec3 mDirection;
		//glm::vec3 mColor;
	};

	enum ParticleType
	{
		POINT,
		MODEL,
		PARTICLE_NUM
	};

	struct ParticleData
	{
		std::vector<MaterialData*> mParticleMaterials;
		std::vector<Mesh*> mParticleModel;
		VertexArray* mpParticleVAO;
		unsigned int mParticlesCount;
		glm::mat4 mModel;
		Texture* mTex;
		ParticleType mType;
		ShaderStorageBuffer* mpParticleDataStorage;
		glm::vec2 mSpeedRange;
		glm::vec2 mLifeRange;
		glm::vec3 mCenter;
		Shader* mpComputeShader;
		glm::vec3 mParticleColor;
		float mPixelSize;
	};
}
