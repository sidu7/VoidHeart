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
		void LoadGameObjectFromFile(std::string path);
		Texture* LoadTexture(std::string path);
		Model* LoadModel(std::string path);
		Mesh* GetShape(Shapes shape);

	private:
		void InitializeShapes();
		void CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Shapes shape);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, Model*> mModelCache;
		std::unordered_map<Shapes, Mesh*> mShapes;
	};
}