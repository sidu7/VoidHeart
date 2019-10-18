#pragma once

namespace Hollow
{
	typedef aiScene RootNode;
	typedef aiNode Node;
	typedef aiMesh ModelMesh;
	typedef aiMaterial ModelMaterial;
	class Texture;
	class Model;
	class Mesh;
	struct Vertex;
	struct MaterialData;
	class VertexBuffer;
	struct Bone;

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

	class HOLLOW_API ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		~ResourceManager() {}

		void Init();
		void CleanUp();
		void LoadLevelFromFile(std::string path);
		void LoadGameObjectFromFile(std::string path);
		Texture* LoadTexture(std::string path);
		std::vector<Mesh*> LoadModel(std::string path);
		std::vector<MaterialData*> LoadMaterials(std::string path);
		std::vector<Bone*> LoadBoneData(std::string path);
		void AddAnimationData(std::string path, std::vector<Bone*>& boneList);
		Mesh* GetShape(Shapes shape);
		FMOD::Sound* LoadSound(const std::string& path, FMOD_MODE type);

	private:
		void InitializeShapes();
		Mesh* CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex = -1, VertexBuffer* vbo = nullptr);
		void ProcessMeshNode(Node* node, const RootNode* scene, const Bone* parent, std::vector<Mesh*>& meshlist, std::vector<Bone*>& boneList);
		Mesh* ProcessMesh(ModelMesh* mesh, const RootNode* scene, std::vector<Bone*>& boneList);
		std::vector<MaterialData*> ProcessMaterials(const RootNode* scene, std::string path);
		Texture* LoadMaterialTexture(ModelMaterial* material, unsigned int textureType, const RootNode* scene, std::string directory);
		inline unsigned int GetBoneIndex(std::string name, std::vector<Bone*>& boneList);
		const RootNode* GetModelRootNodeFromFile(std::string path);
		void ProcessAnimationData(const RootNode* scene, std::vector<Bone*>& boneList);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, std::vector<Mesh*>> mModelCache;
		std::unordered_map<std::string, std::vector<MaterialData*>> mMaterialCache;
		std::unordered_map<std::string, std::vector<Bone*>> mBoneCache;
		std::unordered_map<Shapes, Mesh*> mShapes;
		std::unordered_map<std::string, const RootNode*> mModelRootsCache;

		// Sounds cache, maybe split into SFX and Songs
		std::unordered_map<std::string, FMOD::Sound*> mSoundCache;
	};
}