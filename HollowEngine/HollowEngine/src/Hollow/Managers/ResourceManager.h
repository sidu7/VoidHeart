#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class Texture;
	class Model;
	class Mesh;
	struct Vertex;
	struct MaterialData;
	class VertexBuffer;
	struct Bone;
	struct State;
	class GameObject;
	class Shader;
	struct AnimationData;
	
	enum Shapes
	{
		SPHERE,
		CUBE,
		TEAPOT,
		QUAD,
		CIRCLE,
		AXES,
		WIRECUBE,
		LINE,
		DIRECTION_LINE
	};

	class ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		HOLLOW_API  ~ResourceManager() {}
		  
		HOLLOW_API  void Init();
		HOLLOW_API  void CleanUp();
		HOLLOW_API  void LoadLevelFromFile(std::string path);
		HOLLOW_API  GameObject* LoadGameObjectFromFile(std::string path);
		HOLLOW_API  Texture* LoadTexture(std::string path);
		HOLLOW_API  std::vector<Mesh*> LoadModel(std::string path);
		HOLLOW_API  std::vector<MaterialData*> LoadMaterials(std::string path);
		HOLLOW_API  std::pair<float, std::vector<Bone*>> LoadBoneData(std::string path);
		HOLLOW_API  std::pair<double, double> AddAnimationData(std::string path, std::string name, std::vector<Bone*>& boneList, 
			std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>>& animationList, float factor);
		HOLLOW_API  Mesh* GetShape(Shapes shape);
		HOLLOW_API  FMOD::Sound* LoadSound(const std::string& path, FMOD_MODE type);
		HOLLOW_API  std::vector<State*> ReadStateMachineFile(std::string path);
		HOLLOW_API  Shader* LoadShader(std::string path);

	private:
		void InitializeShapes();
		Mesh* CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex = -1, VertexBuffer* vbo = nullptr);
		void ProcessMeshNode(aiNode* node, const aiScene* scene, const Bone* parent, std::vector<Mesh*>& meshlist, std::vector<Bone*>& boneList, float maxm);
		Mesh* ProcessMesh(aiMesh* mesh, std::vector<Bone*>& boneList, float maxm);
		std::vector<MaterialData*> ProcessMaterials(const aiScene* scene, std::string path);
		Texture* LoadMaterialTexture(aiMaterial* material, unsigned int textureType, const aiScene* scene, std::string directory);
		inline unsigned int GetBoneIndex(std::string name, std::vector<Bone*>& boneList);
		const aiScene* GetModelRootNodeFromFile(std::string path, unsigned int flags);
		std::pair<double, double> ProcessAnimationData(const aiScene* scene, std::vector<Bone*>& boneList, 
			std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>>& animationList, float maxm, std::string name);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, std::vector<Mesh*>> mModelCache;
		std::unordered_map<std::string, std::vector<MaterialData*>> mMaterialCache;
		std::unordered_map<std::string, std::pair<float,std::vector<Bone*>>> mBoneCache;
		std::unordered_map<Shapes, Mesh*> mShapes;
		std::unordered_map<std::string, const aiScene*> mModelRootsCache;
		std::unordered_map<std::string, std::vector<State*>> mStateFileCache;
		std::unordered_map<std::string, Shader*> mShaderCache;
		std::unordered_map<std::string, std::string> mCachedGameObjectsMap;

		// Sounds cache, maybe split into SFX and Songs
		std::unordered_map<std::string, FMOD::Sound*> mSoundCache;

		//Model Loader
		Assimp::Importer* importer;
	};
}