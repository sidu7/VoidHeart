#pragma once

namespace Hollow
{
	class Texture;
	class Model;
	class Mesh;
	struct Vertex;
	struct MaterialData;

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
		std::vector<MaterialData> LoadMaterial(std::string path);
		Mesh* GetShape(Shapes shape);

	private:
		void InitializeShapes();
		Mesh* CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex = -1);
		void ProcessMeshNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshlist);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MaterialData> ProcessMaterials(const aiScene* scene);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, std::vector<Mesh*>> mModelCache;
		std::unordered_map<std::string, std::vector<MaterialData>> mMaterialCache;
		std::unordered_map<std::string, const aiScene*> mModelSceneCache;
		std::unordered_map<Shapes, Mesh*> mShapes;
	};
}