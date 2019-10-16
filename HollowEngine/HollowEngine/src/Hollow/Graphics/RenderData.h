#pragma once
#include "glm/glm.hpp"

namespace Hollow {	

	class Shader;
	class Material;
	class Mesh;
	class FrameBuffer;
	class VertexBuffer;
	class VertexArray;
	class Texture;
	enum ParticleType;
	enum DebugShape;

	struct RenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		Material* mpMaterial;
		bool mCastShadow;
		Shader* mpShader;
	};

	enum DebugShape
	{
		DEBUGLINE,
		DEBUGQUAD,
		DEBUGAXES,
		DEBUGCIRCLE,
		DEBUGCUBE,
		DEBUGSPHERE
	};

	struct DebugRenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		GLenum mDrawCommand;
		DebugShape mType;
		glm::vec3 mColor;
	};

	struct LightData
	{
		glm::vec3 mPosition;
		float mRadius;
		glm::vec3 mColor;
		bool mCastShadow;
		FrameBuffer* mpShadowMap;
		glm::mat4 mShadowMatrix;
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
	};
}