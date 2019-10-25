#pragma once

namespace Hollow
{
	class Mesh;
	class VertexArray;
	class VertexBuffer;
	class Texture;
	class ShaderStorageBuffer;

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
		ShaderStorageBuffer* mpShaderStorage;
	};
}
