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

		/// <summary>
		/// Initializes the ParticleEmitter component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the ParticleEmitter component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the ParticleEmitter component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current ParticleEmitter component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the ParticleEmitter Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	private:
		unsigned int mDType;
		std::string mComputeShaderPath;
		
	public:
		unsigned long mCount;
		std::string mDTexturePath;
		
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