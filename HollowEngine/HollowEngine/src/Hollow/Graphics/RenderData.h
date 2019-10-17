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
	enum Shapes;

	struct RenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		Material* mpMaterial;
		bool mCastShadow;
		Shader* mpShader;
	};

	struct DebugRenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		GLenum mDrawCommand;
		Shapes mType;
		glm::vec3 mColor;

		DebugRenderData(std::vector<Mesh*> mesh,glm::mat4 model,GLenum drawCommand, Shapes shape, glm::vec3 color)
			: mpMeshes(mesh), mpModel(model), mDrawCommand(drawCommand), mType(shape), mColor(color) { }
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

	struct MaterialData
	{
		Texture* mpDiffuse;
		Texture* mpSpecular;
		Texture* mpNormal;
		Texture* mpHeight;
	};
}