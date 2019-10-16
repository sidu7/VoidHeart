#pragma once

namespace Hollow
{
	class Texture;
	class Model;
	class Mesh;
	struct Vertex;

	enum Shapes
	{
		SPHERE,
		CUBE,
		TEAPOT,
		QUAD
	};

	class HOLLOW_API ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		~ResourceManager() {}

		void Init();
		void CleanUp();
		void LoadGameObjectFromFile(std::string path);
		Texture* LoadTexture(std::string path);
		std::vector<Mesh*> LoadModel(std::string path);
		Mesh* GetShape(Shapes shape);
		FMOD::Sound* LoadSound(const std::string& path, FMOD_MODE type);

	private:
		void InitializeShapes();
		Mesh* CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		void ProcessMeshNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshlist);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, std::vector<Mesh*>> mModelCache;
		std::unordered_map<Shapes, Mesh*> mShapes;

		// Sounds cache, maybe split into SFX and Songs
		std::unordered_map<std::string, FMOD::Sound*> mSoundCache;
	};
}