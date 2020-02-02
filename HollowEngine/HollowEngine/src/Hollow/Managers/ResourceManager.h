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
#define MESH_SHAPE(name) name,
#include "Hollow/Enums/MeshShapes.enum"
#undef MESH_SHAPE
		SHAPE_NUM
	};

	/// <summary>
	/// Class Resource Manager.
	/// </summary>
	class ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		/// <summary>
		/// Initializes manager data.
		/// </summary> 
		HOLLOW_API  void Init();
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API  void CleanUp();
		
		/// <summary>
		/// Loads the json file at path.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>std.string.</returns>
		HOLLOW_API  std::string LoadJSONFile(std::string path);
		
		/// <summary>
		/// Loads the game object from file.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>Hollow.GameObject *.</returns>
		HOLLOW_API  GameObject* LoadGameObjectFromFile(std::string path);

		/// <summary>
		/// Loads the game object from file at the specified position.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <param name="pos">The position.</param>
		/// <param name="scale">The scale of the object (optional).</param>
		/// <returns>Hollow.GameObject *.</returns>
		HOLLOW_API  GameObject* LoadPrefabAtPosition(std::string prefabName, glm::vec3 pos);

		HOLLOW_API  GameObject* LoadScaledPrefabAtPosition(std::string prefabName, glm::vec3 pos, glm::vec3 scale);
		
		/// <summary>
		/// Loads the texture.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>Hollow.Texture *.</returns>
		HOLLOW_API  Texture* LoadTexture(std::string path);
		
		/// <summary>
		/// Loads the model.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>List of Meshes.</returns>
		HOLLOW_API  std::vector<Mesh*> LoadModel(std::string path);
		
		/// <summary>
		/// Loads the materials.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>List of material data.</returns>
		HOLLOW_API  std::vector<MaterialData*> LoadMaterials(std::string path);
		
		/// <summary>
		/// Loads the bone data.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>pair(modelFactor,List of Bones).</returns>
		HOLLOW_API  std::pair<float, std::vector<Bone*>> LoadBoneData(std::string path);
		
		/// <summary>
		/// Adds animation data to the list of bones.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <param name="name">The name.</param>
		/// <param name="boneList">The bone list.</param>
		/// <param name="animationList">The animation list.</param>
		/// <param name="factor">The factor.</param>
		/// <returns>pair(animation duration, ticks per second).</returns>
		HOLLOW_API  std::pair<double, double> AddAnimationData(std::string path, std::string name, std::vector<Bone*>& boneList,
			std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>>& animationList, float factor);
		
		/// <summary>
		/// Gets the shape mesh.
		/// </summary>
		/// <param name="type">The type name.</param>
		/// <returns>Hollow.Mesh *.</returns>
		HOLLOW_API  Mesh* GetShape(std::string type);
		
		/// <summary>
		/// Gets the shape mesh.
		/// </summary>
		/// <param name="type">The type.</param>
		/// <returns>Hollow.Mesh *.</returns>
		HOLLOW_API  Mesh* GetShape(Shapes type);
		
		/// <summary>
		/// Loads the sound.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <param name="type">The type.</param>
		/// <returns>FMOD.Sound *.</returns>
		HOLLOW_API  FMOD::Sound* LoadSound(const std::string& path, FMOD_MODE type);
		
		/// <summary>
		/// Parses the state machine file.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>List of states.</returns>
		HOLLOW_API  std::vector<State*> ReadStateMachineFile(std::string path);
		
		/// <summary>
		/// Loads the shader.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>Hollow.Shader *.</returns>
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
		std::unordered_map<std::string, std::string> mJSONFileCache;
		std::unordered_map<std::string, Shapes> mShapeEnumMap;

		// Sounds cache, maybe split into SFX and Songs
		std::unordered_map<std::string, FMOD::Sound*> mSoundCache;

		//Model Loader
		Assimp::Importer* importer;
	};
}