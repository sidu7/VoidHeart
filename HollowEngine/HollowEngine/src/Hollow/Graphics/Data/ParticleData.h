#pragma once

namespace Hollow
{
	class Mesh;
	class VertexArray;
	class VertexBuffer;
	class Texture;

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
	};
}
