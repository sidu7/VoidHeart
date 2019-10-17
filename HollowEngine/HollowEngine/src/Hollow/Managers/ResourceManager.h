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
		Mesh* GetShape(Shapes shape);

	private:
		void InitializeShapes();
		Mesh* CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex = -1);
		void ProcessMeshNode(Node* node, const RootNode* scene, std::vector<Mesh*>& meshlist);
		Mesh* ProcessMesh(ModelMesh* mesh, const RootNode* scene);
		std::vector<MaterialData*> ProcessMaterials(const RootNode* scene, std::string path);
		Texture* LoadMaterialTexture(ModelMaterial* material, unsigned int textureType, const RootNode* scene, std::string directory);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, std::vector<Mesh*>> mModelCache;
		std::unordered_map<std::string, std::vector<MaterialData*>> mMaterialCache;
		std::unordered_map<Shapes, Mesh*> mShapes;
	};
}