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
	struct MaterialData;

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
		std::string mDModelPath;
		
	public:
		std::string mDTexturePath;
		unsigned long mDrawCount;
		unsigned long mMaxCount;
		unsigned long mEmissionRate;
		unsigned long mCount;
		
		// For Point particles
		Texture* mTexture;
		glm::mat4 mModelMatrix;
		float mPixelSize;

		// For Model particles
		std::vector<Mesh*> mParticleModel;
		std::vector<MaterialData*> mParticleMaterials;

		ParticleType mType;
		bool mActive;
		glm::vec2 mSpeedRange;
		glm::vec2 mLifeRange;
		glm::vec2 mSizeRange;
		glm::vec3 mCenterOffset;
		glm::vec3 mAreaOfEffect;
		glm::vec3 mParticleColor;
		Shader* mpComputeShader;
		ShaderStorageBuffer* mpParticleStorage;
		VertexArray* mpParticleVAO;
		//TODO: direction data and scales
	};
}