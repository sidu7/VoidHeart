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
	struct Particle;
	class Shader;

	class ParticleEmitter : public Component
	{
		REGISTERCOMPONENT(ParticleEmitter)
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		HOLLOW_API void DebugDisplay() override;

	private:
		unsigned int mDType;
		std::string mComputeShaderPath;
		std::string mDTexturePath;
		
	public:
		unsigned long mCount;
		
		// For Point particles
		VertexArray* mpParticlePositionVAO;
		Texture* mTexture;
		glm::mat4 mModelMatrix;
		ShaderStorageBuffer* mpParticleStorage;
		Shader* mpComputeShader;

		ParticleType mType;
		glm::vec2 mSpeedRange;
		glm::vec2 mLifeRange;
		glm::vec3 mCenterOffset;
		glm::vec3 mAreaOfEffect;
		float mPixelSize;
		//TODO: direction data and scales
	};
}