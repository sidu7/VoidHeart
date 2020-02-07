#include <hollowpch.h>
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "AudioManager.h"

#include "Hollow/Core/GameObjectFactory.h"

#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/ElementArrayBuffer.h"
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/Data/RenderData.h"
#include "Hollow/Graphics/Data/Bone.h"
#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Shader.h"

#include "Hollow/Core/Data/StateData.h"
#include "Hollow/Components/Body.h"
#include "Hollow/Components/Transform.h"
#include "PhysicsManager.h"

namespace Hollow
{ 
	void ResourceManager::Init()
	{
		InitializeShapes();
		importer = new Assimp::Importer();
		
		{
#define MESH_SHAPE(name) mShapeEnumMap[#name] = Shapes::name;
#include "Hollow/Enums/MeshShapes.enum"
#undef MESH_SHAPE
		}
	}

	void ResourceManager::CleanUp()
	{
		delete importer;
		std::for_each(mTextureCache.begin(), mTextureCache.end(), [](std::pair<std::string, Texture*> value) { delete value.second; });
		std::for_each(mModelCache.begin(), mModelCache.end(), [](std::pair<std::string, std::vector<Mesh*>> value) { for (Mesh* mesh : value.second) delete mesh; });
		std::for_each(mShapes.begin(), mShapes.end(), [](std::pair<Shapes, Mesh*> value) { delete value.second; });
		std::for_each(mSoundCache.begin(), mSoundCache.end(), [](std::pair<std::string, FMOD::Sound*> value) {value.second->release(); });
		std::for_each(mBoneCache.begin(), mBoneCache.end(), [](std::pair<std::string, std::pair<float,std::vector<Bone*>>> value) { for (Bone* bone : value.second.second) delete bone; });
		std::for_each(mMaterialCache.begin(), mMaterialCache.end(), [](std::pair<std::string, std::vector<MaterialData*>> value) { for (MaterialData* material : value.second) delete material; });
		std::for_each(mModelRootsCache.begin(), mModelRootsCache.end(), [](std::pair<std::string, const aiScene*> value) { delete value.second; });
	}

	std::string ResourceManager::LoadJSONFile(std::string path)
	{
		if(mJSONFileCache.find(path) == mJSONFileCache.end())
		{
			std::ifstream file(path);
			std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
			mJSONFileCache[path] = contents;
		}
		return mJSONFileCache[path];		
	}

	GameObject* ResourceManager::LoadGameObjectFromFile(std::string path)
	{
		if (mCachedGameObjectsMap.find(path) == mCachedGameObjectsMap.end())
		{
			std::ifstream file(path); 
			std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
			file.close();

			mCachedGameObjectsMap[path] = contents;
		}
			
		rapidjson::Document root; 
		root.Parse(mCachedGameObjectsMap[path].c_str());
		if (!root.IsObject()) { HW_CORE_ERROR("Error reading JSON file {0}", path); }

		GameObject* pNewGameObject = GameObjectFactory::Instance().LoadObject(root.GetObject());

		if (pNewGameObject)
		{
			GameObjectManager::Instance().AddGameObject(pNewGameObject);
			return pNewGameObject;
		}
		
		return nullptr;
	}

	GameObject* ResourceManager::LoadPrefabAtPosition(std::string prefabName, glm::vec3 pos)
	{
		GameObject* pGo = LoadGameObjectFromFile(std::string("Resources/Prefabs/") + prefabName + std::string(".json"));
		Body* pBody = pGo->GetComponent<Body>();
		pBody->mPosition = pos;
		return pGo;
	}

	GameObject* ResourceManager::LoadScaledPrefabAtPosition(std::string prefabName, glm::vec3 pos, glm::vec3 scale)
	{
		GameObject* pGo = LoadGameObjectFromFile(std::string("Resources/Prefabs/") + prefabName + std::string(".json"));
		Body* pBody = pGo->GetComponent<Body>();
		pBody->mPosition = pos;
		
		Transform* pTr = pGo->GetComponent<Transform>();
		pTr->mScale = scale;

		PhysicsManager::Instance().UpdateScale(pGo);
		
		return pGo;
	}

	Texture* ResourceManager::LoadTexture(std::string path)
	{
		//Check in cache
		if (mTextureCache.find(path) != mTextureCache.end())
		{
			return mTextureCache[path];
		}

		Texture* texture = new Texture(path);
		mTextureCache[path] = texture;
		return texture;
	}

	std::vector<Mesh*> ResourceManager::LoadModel(std::string path)
	{
		//Check in cache
		if (mModelCache.find(path) == mModelCache.end())
		{
			const aiScene* scene = GetModelRootNodeFromFile(path,Model_Flags);

			float maxm = std::numeric_limits<float>::min();
			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				aiMesh* mesh = scene->mMeshes[i];
				for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
				{
					aiVector3D vert = mesh->mVertices[j];
					maxm = std::max(vert.x, maxm);
					maxm = std::max(vert.y, maxm);
					maxm = std::max(vert.z, maxm);
				}
			}
			std::vector<Mesh*> meshes;
			std::vector<Bone*> bones;
			ProcessMeshNode(scene->mRootNode, scene, nullptr, meshes, bones, maxm);

			mModelCache[path] = meshes;
			mBoneCache[path] = std::make_pair(maxm,bones);
		}

		return mModelCache[path];
	}

	std::vector<MaterialData*> ResourceManager::LoadMaterials(std::string path)
	{
		//Check in cache
		if (mMaterialCache.find(path) == mMaterialCache.end())
		{
			const aiScene* scene = GetModelRootNodeFromFile(path,Model_Flags);
			mMaterialCache[path] = ProcessMaterials(scene, path);
		}
		return mMaterialCache[path];
	}

	std::pair<float,std::vector<Bone*>> ResourceManager::LoadBoneData(std::string path)
	{
		if (mBoneCache.find(path) == mBoneCache.end())
		{
			LoadModel(path);
		}
		return mBoneCache[path];
	}

	std::pair<double, double> ResourceManager::AddAnimationData(std::string path, std::string name, std::vector<Bone*>& boneList, std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>>& animationList, float factor)
	{
		const aiScene* scene = GetModelRootNodeFromFile(path,Animation_Flags);
		
		return ProcessAnimationData(scene, boneList, animationList, factor, name);
	}

	void ResourceManager::ProcessMeshNode(aiNode* node, const aiScene* scene, const Bone* parent, std::vector<Mesh*>& meshlist, std::vector<Bone*>& boneList, float maxm)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshlist.push_back(ProcessMesh(mesh, boneList, maxm));
		}

		Bone* child = new Bone();
		child->mIndex = static_cast<unsigned int>(boneList.size());
		glm::mat4 tt = *(glm::mat4*)(&node->mTransformation.Transpose());
		tt[3].x /= maxm;
		tt[3].y /= maxm;
		tt[3].z /= maxm;
		child->mTransformation = tt;
		child->mName = node->mName.data;
		child->mOffset = glm::mat4(1.0f);
		if (parent)
		{
			child->mParentIndex = parent->mIndex;
		}
		else
		{
			child->mParentIndex = -1;
		}
		boneList.push_back(child);

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessMeshNode(node->mChildren[i], scene, child, meshlist, boneList, maxm);
		}
	}

	Mesh* ResourceManager::ProcessMesh(aiMesh* mesh, std::vector<Bone*>& boneList, float maxm)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int materialIndex = -1;
		std::vector<BoneData> bonedata;

		//Processing vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z) / maxm;
			if (mesh->mNormals)
			{
				vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
			else
			{
				vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
			}


			if (mesh->mTextureCoords[0])
			{
				vertex.tex = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertex.tex = glm::vec2(0.0f, 0.0f);
			}

			if (mesh->mTangents)
			{
				vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			}
			else
			{
				vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			vertices.push_back(vertex);
			bonedata.push_back(BoneData(0, 0.0f));
		}

		//Processing indices
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//Processing textures
		if (mesh->mMaterialIndex >= 0)
		{
			materialIndex = mesh->mMaterialIndex;
		}

		//Process Bones index and weights
		for (unsigned int i = 0; i < mesh->mNumBones; ++i)
		{
			aiBone* bone = mesh->mBones[i];
			unsigned int boneIndex = GetBoneIndex(bone->mName.data, boneList);
			for (unsigned int j = 0; j < bone->mNumWeights; ++j)
			{
				unsigned int index = bone->mWeights[j].mVertexId;
				bonedata[index].AddBone(boneIndex, bone->mWeights[j].mWeight);
			}
			glm::mat4 tt = *(glm::mat4*)(&bone->mOffsetMatrix.Transpose());
			tt[3].x /= maxm;
			tt[3].y /= maxm;
			tt[3].z /= maxm;
			boneList[boneIndex]->mOffset = tt;
		}

		VertexBuffer boneVBO;
		boneVBO.AddData(&bonedata[0], bonedata.size(), sizeof(BoneData));

		return CreateMesh(vertices, indices, materialIndex, &boneVBO);
	}

	std::vector<MaterialData*> ResourceManager::ProcessMaterials(const aiScene* scene, std::string path)
	{
		std::vector<MaterialData*> materials;
		path = path.substr(0, path.find_last_of('/'));
		if (scene->HasMaterials())
		{
			for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
			{
				MaterialData* materialdata = new MaterialData();
				// Note: Assuming Material has only 1 texture of each type, rarely is otherwise
				aiMaterial* material = scene->mMaterials[i];
				materialdata->mpDiffuse = LoadMaterialTexture(material, aiTextureType_DIFFUSE, scene, path);
				materialdata->mpSpecular = LoadMaterialTexture(material, aiTextureType_SPECULAR, scene,path);
				materialdata->mpNormal = LoadMaterialTexture(material, aiTextureType_NORMALS, scene,path);
				materialdata->mpHeight = LoadMaterialTexture(material, aiTextureType_HEIGHT, scene,path);

				materials.push_back(materialdata);
			}
		}
		return materials;
	}

	Texture* ResourceManager::LoadMaterialTexture(aiMaterial* material, unsigned int textureType, const aiScene* scene, std::string directory)
	{
		aiString str;
		unsigned int count = material->GetTextureCount((aiTextureType)textureType);
		if (count == 0)
		{
			return nullptr;
		}
		material->GetTexture((aiTextureType)textureType, 0, &str); // only reading 1 texture of each type at 0 index
		const aiTexture* tex = scene->GetEmbeddedTexture(str.C_Str());
		Texture* texture = nullptr;
		if (tex && tex->mHeight == 0)
		{
			texture = new Texture(tex->pcData, tex->mWidth);
		}
		else
		{
			std::string fileName = str.C_Str();
			fileName = fileName.substr(fileName.find_last_of('\\') + 1, fileName.size());
			texture = new Texture(directory + '/' + fileName);
		}
		return texture;
	}

	unsigned int ResourceManager::GetBoneIndex(std::string name, std::vector<Bone*>& boneList)
	{
		for (unsigned int i = 0; i < boneList.size(); ++i)
		{
			if (boneList[i]->mName == name)
			{
				return boneList[i]->mIndex;
			}
		}
		return -1;
		HW_CORE_ERROR("Bone index not found");
	}

	const aiScene* ResourceManager::GetModelRootNodeFromFile(std::string path, unsigned int flags)
	{
		if (mModelRootsCache.find(path) == mModelRootsCache.end())
		{
			const aiScene* scene = importer->ReadFile(path,flags);
			importer->GetOrphanedScene();
			if (!scene || !scene->mRootNode)
			{
				HW_CORE_ERROR("Model file {0} could not be loaded", path);
			}

			mModelRootsCache[path] = scene;
			return scene;
		}
		return mModelRootsCache[path];
	}

	std::pair<double, double> ResourceManager::ProcessAnimationData(const aiScene* scene, std::vector<Bone*>& boneList, 
		std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>>& animationList, float maxm, std::string name)
	{
		for (unsigned int i = 0; i < boneList.size(); ++i)
		{
			auto& map = animationList[i];
			map[name] = std::make_pair(false, AnimationData());
		}

		std::pair<double, double> ticks_duration;
		for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
		{
			aiAnimation* animation = scene->mAnimations[i];
			ticks_duration.first = animation->mTicksPerSecond;
			ticks_duration.second = animation->mDuration;
			for (unsigned int j = 0; j < animation->mNumChannels; ++j)
			{
				unsigned int index = GetBoneIndex(animation->mChannels[j]->mNodeName.data, boneList);
				AnimationData animData;
				animData.mDuration = ticks_duration.first;
				animData.mTicksPerSec = ticks_duration.second;
				//Key Positions
				for (unsigned int k = 0; k < animation->mChannels[j]->mNumPositionKeys; ++k)
				{
					animData.mKeyPositions.push_back(std::make_pair(animation->mChannels[j]->mPositionKeys[k].mTime, *(glm::vec3*) & (animation->mChannels[j]->mPositionKeys[k].mValue / maxm)));
				}

				//Key Rotations
				for (unsigned int k = 0; k < animation->mChannels[j]->mNumRotationKeys; ++k)
				{
					aiQuaternion aiquat = animation->mChannels[j]->mRotationKeys[k].mValue;
					glm::quat quat(aiquat.w, aiquat.x, aiquat.y, aiquat.z);
					animData.mKeyRotations.push_back(std::make_pair(animation->mChannels[j]->mRotationKeys[k].mTime, quat));
				}

				//Key Scalings
				for (unsigned int k = 0; k < animation->mChannels[j]->mNumScalingKeys; ++k)
				{
					animData.mKeyScalings.push_back(std::make_pair(animation->mChannels[j]->mScalingKeys[k].mTime, *(glm::vec3*) & (animation->mChannels[j]->mScalingKeys[k].mValue)));
				}
				animationList[index][name] = std::make_pair(true, animData);
			}
		}
		return ticks_duration;
	}


	Mesh* ResourceManager::GetShape(std::string type)
	{
		/*if (mShapes.find(shape) == mShapes.end())
		{
			HW_CORE_ERROR("Shape {0} not found", shape);
		}*/
		return mShapes[mShapeEnumMap[type]];
	}

	Mesh* ResourceManager::GetShape(Shapes type)
	{
		/*if (mShapes.find(shape) == mShapes.end())
		{
			HW_CORE_ERROR("Shape {0} not found", shape);
		}*/
		return mShapes[type];
	}

	FMOD::Sound* ResourceManager::LoadSound(const std::string& path, FMOD_MODE type)
	{
		//Check in cache
		if (mSoundCache.find(path) != mSoundCache.end())
		{
			return mSoundCache[path];
		}

		// Create sound
		FMOD::Sound* pSound;
		AudioManager::Instance().mpSystem->createSound(path.c_str(), type, 0, &pSound);
		if (pSound)
		{
			mSoundCache[path] = pSound;
		}

		return pSound;
	}

	std::vector<State*> ResourceManager::ReadStateMachineFile(std::string path)
	{
		if (mStateFileCache.find(path) == mStateFileCache.end())
		{
			std::string contents = LoadJSONFile(path);
			PARSE_JSON_FILE(contents.c_str());
						
			std::vector<State*> states;
			rapidjson::Value::Array stateList = root["States"].GetArray();

			for (unsigned int i = 0; i < stateList.Size(); ++i)
			{
				State* newState = new State();
				newState->mName = stateList[i].GetString();
				newState->mIndex = i;
				states.emplace_back(newState);
			}

			rapidjson::Value::Array conditions = root["Conditions"].GetArray();

			for (unsigned int i = 0; i < conditions.Size(); ++i)
			{
				rapidjson::Value::Object statecondition = conditions[i].GetObject();

				State* state = states[State::FindState(states,statecondition["State"].GetString())];

				state->mIsLoop = statecondition["IsLoop"].GetBool();
				state->mEvents = JSONHelper::GetArray<std::string>(statecondition["Events"].GetArray());
				rapidjson::Value::Array eventState = statecondition["EventStates"].GetArray();
				state->mEventStates.reserve(eventState.Size());
				for (unsigned int i = 0; i < eventState.Size(); ++i)
				{
					state->mEventStates.emplace_back(State::FindState(states,eventState[i].GetString()));
				}

				state->mInputs = JSONHelper::GetArray<std::string>(statecondition["Inputs"].GetArray());
				rapidjson::Value::Array inputState = statecondition["InputStates"].GetArray();
				state->mInputStates.reserve(inputState.Size());
				for (unsigned int i = 0; i < inputState.Size(); ++i)
				{
					state->mInputStates.emplace_back(State::FindState(states,inputState[i].GetString()));
				}
				state->mInputConditions = JSONHelper::GetArray<State::StateInputCondition>(statecondition["InputCondition"].GetArray());
			}
				
			mStateFileCache[path] = states;
		}

		return mStateFileCache[path];
	}

	Shader* ResourceManager::LoadShader(std::string path)
	{
		if (mShaderCache.find(path) == mShaderCache.end())
		{
			Shader* shader = new Shader(path.c_str());
			mShaderCache[path] = shader;
		}
		return mShaderCache[path];
	}

	void ResourceManager::InitializeShapes()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		//Axes
		{
			std::vector<glm::vec3> verts;
			//X axis
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			verts.push_back(glm::vec3(0.8f, 0.05f, 0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			verts.push_back(glm::vec3(0.8f, -0.05f, 0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			verts.push_back(glm::vec3(0.8f, 0.05f, -0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			verts.push_back(glm::vec3(0.8f, -0.05f, -0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			//Y axis
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

			verts.push_back(glm::vec3(0.05f, 0.8f, 0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

			verts.push_back(glm::vec3(-0.05f, 0.8f, 0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

			verts.push_back(glm::vec3(0.05f, 0.8f, -0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

			verts.push_back(glm::vec3(-0.05f, 0.8f, -0.05f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));


			//Z axis
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			verts.push_back(glm::vec3(0.05f, 0.05f, 0.8f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			verts.push_back(glm::vec3(-0.05f, 0.05f, 0.8f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			verts.push_back(glm::vec3(0.05f, -0.05f, 0.8f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			verts.push_back(glm::vec3(-0.05f, -0.05f, 0.8f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int ind[] = {
				0, 1, 1, 2, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 4, 5, 5, 3,			// X-axis
				6, 7, 7, 8, 7, 9, 7, 10, 7, 11,	8, 9, 8, 10, 10, 11, 11, 9,		// Y-axis
				12, 13, 13, 14, 13, 15, 13, 16, 13, 17, 14, 15, 14, 16, 16, 17, 17, 15	//Z-axis
			};

			VertexBuffer* vbo = new VertexBuffer();
			VertexArray* vao = new VertexArray();
			ElementArrayBuffer* ebo = new ElementArrayBuffer();
			ebo->AddData(&ind[0], 54, sizeof(unsigned int));
			vao->AddBuffer(*vbo);
			vbo->AddData(&verts[0], 18, 4 * sizeof(glm::vec3));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->AddLayout();
			vao->Unbind();
			Mesh* mesh = new Mesh();
			mesh->mpVAO = vao;
			mesh->mpVBO = vbo;
			mesh->mpEBO = ebo;
			mShapes[AXES] = mesh;
		}

		//Line
		{
			std::vector<glm::vec3> verts;
			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

			unsigned int ind[] = {
				0, 1
			};

			VertexBuffer* vbo = new VertexBuffer();
			VertexArray* vao = new VertexArray();
			ElementArrayBuffer* ebo = new ElementArrayBuffer();
			ebo->AddData(&ind[0], 2, sizeof(unsigned int));
			vao->AddBuffer(*vbo);
			vbo->AddData(&verts[0], 2, sizeof(glm::vec3));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->AddLayout();
			vao->Unbind();
			Mesh* mesh = new Mesh();
			mesh->mpVAO = vao;
			mesh->mpVBO = vbo;
			mesh->mpEBO = ebo;
			mShapes[LINE] = mesh;
		}

		//Directional line
		{
			std::vector<glm::vec3> verts;

			verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			verts.push_back(glm::vec3(0.8f, 0.05f, 0.05f));
			verts.push_back(glm::vec3(0.8f, -0.05f, 0.05f));
			verts.push_back(glm::vec3(0.8f, 0.05f, -0.05f));
			verts.push_back(glm::vec3(0.8f, -0.05f, -0.05f));

			unsigned int ind[] = {
				0, 1, 1, 2, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 4, 5, 5, 3
			};

			VertexBuffer* vbo = new VertexBuffer();
			VertexArray* vao = new VertexArray();
			ElementArrayBuffer* ebo = new ElementArrayBuffer();
			ebo->AddData(&ind[0], 18, sizeof(unsigned int));
			vao->AddBuffer(*vbo);
			vbo->AddData(&verts[0], 6, sizeof(glm::vec3));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->AddLayout();
			vao->Unbind();
			Mesh* mesh = new Mesh();
			mesh->mpVAO = vao;
			mesh->mpVBO = vbo;
			mesh->mpEBO = ebo;
			mShapes[DIRECTION_LINE] = mesh;

		}

		//WireCube
		{
			std::vector<glm::vec3> verts;

			verts.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
			verts.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
			verts.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
			verts.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
			verts.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
			verts.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
			verts.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
			verts.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));

			unsigned int ind[] = {
				0, 1, 0, 2, 1, 3, 2, 3, 4, 5, 4, 6, 5, 7, 6, 7, 0, 4, 1, 5, 2, 6, 3, 7
			};

			VertexBuffer* vbo = new VertexBuffer();
			VertexArray* vao = new VertexArray();
			ElementArrayBuffer* ebo = new ElementArrayBuffer();
			ebo->AddData(&ind[0], 24, sizeof(unsigned int));
			vao->AddBuffer(*vbo);
			vbo->AddData(&verts[0], 8, sizeof(glm::vec3));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->AddLayout();
			vao->Unbind();
			Mesh* mesh = new Mesh();
			mesh->mpVAO = vao;
			mesh->mpVBO = vbo;
			mesh->mpEBO = ebo;
			mShapes[WIRECUBE] = mesh;
		}

		//Circle
		{
			std::vector<glm::vec3> verts;

			verts.push_back(glm::vec3(0, 1, 0));
			verts.push_back(glm::vec3(0.19509, 0.980785, 0));
			verts.push_back(glm::vec3(0.382683, 0.92388, 0));
			verts.push_back(glm::vec3(0.55557, 0.83147, 0));
			verts.push_back(glm::vec3(0.707107, 0.707107, 0));
			verts.push_back(glm::vec3(0.831469, 0.55557, 0));
			verts.push_back(glm::vec3(0.923879, 0.382684, 0));
			verts.push_back(glm::vec3(0.980785, 0.195091, 0));
			verts.push_back(glm::vec3(1, 3.13916e-07, 0));
			verts.push_back(glm::vec3(0.980785, -0.19509, 0));
			verts.push_back(glm::vec3(0.92388, -0.382683, 0));
			verts.push_back(glm::vec3(0.83147, -0.55557, 0));
			verts.push_back(glm::vec3(0.707107, -0.707106, 0));
			verts.push_back(glm::vec3(0.555571, -0.831469, 0));
			verts.push_back(glm::vec3(0.382684, -0.923879, 0));
			verts.push_back(glm::vec3(0.195091, -0.980785, 0));
			verts.push_back(glm::vec3(6.27833e-07, -1, 0));
			verts.push_back(glm::vec3(-0.19509, -0.980785, 0));
			verts.push_back(glm::vec3(-0.382683, -0.92388, 0));
			verts.push_back(glm::vec3(-0.55557, -0.83147, 0));
			verts.push_back(glm::vec3(-0.707106, -0.707107, 0));
			verts.push_back(glm::vec3(-0.831469, -0.555571, 0));
			verts.push_back(glm::vec3(-0.923879, -0.382684, 0));
			verts.push_back(glm::vec3(-0.980785, -0.195091, 0));
			verts.push_back(glm::vec3(-1, -9.41749e-07, 0));
			verts.push_back(glm::vec3(-0.980785, 0.195089, 0));
			verts.push_back(glm::vec3(-0.92388, 0.382683, 0));
			verts.push_back(glm::vec3(-0.83147, 0.555569, 0));
			verts.push_back(glm::vec3(-0.707108, 0.707106, 0));
			verts.push_back(glm::vec3(-0.555571, 0.831469, 0));
			verts.push_back(glm::vec3(-0.382685, 0.923879, 0));
			verts.push_back(glm::vec3(-0.195092, 0.980785, 0));

			unsigned int ind[32];
			for (int i = 0; i < 32; ++i) ind[i] = i;

			VertexBuffer* vbo = new VertexBuffer();
			VertexArray* vao = new VertexArray();
			ElementArrayBuffer* ebo = new ElementArrayBuffer();
			ebo->AddData(&ind[0], 32, sizeof(unsigned int));
			vao->AddBuffer(*vbo);
			vbo->AddData(&verts[0], 32, sizeof(glm::vec3));
			vao->Push(3, GL_FLOAT, sizeof(float));
			vao->AddLayout();
			vao->Unbind();
			Mesh* mesh = new Mesh();
			mesh->mpVAO = vao;
			mesh->mpVBO = vbo;
			mesh->mpEBO = ebo;
			mShapes[CIRCLE] = mesh;
		}

		//Quad
		{
			Vertex v;
			v.position = glm::vec3(-0.5f, 0.5f, 0.0f);
			v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			v.tex = glm::vec2(0.0f, 0.0f);
			v.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices.push_back(v);

			v.position = glm::vec3(0.5f, 0.5f, 0.0f);
			v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			v.tex = glm::vec2(1.0f, 0.0f);
			vertices.push_back(v);

			v.position = glm::vec3(-0.5f, -0.5f, 0.0f);
			v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			v.tex = glm::vec2(0.0f, 1.0f);
			vertices.push_back(v);

			v.position = glm::vec3(0.5f, -0.5f, 0.0f);
			v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			v.tex = glm::vec2(1.0f, 1.0f);
			vertices.push_back(v);

			//for (unsigned int i = 0; i < 4; ++i)
			//{
			//	Vertex v;
			//	int x = i == 0 || i == 3 ? -1 : 1;
			//	int y = i < 2 ? -1 : 1;
			//	v.position = glm::vec3(0.5f * x, 0.5f * y, 0.0f);
			//	v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			//	v.tex = glm::vec2(x == -1 ? 0.0f : 1.0f, y == -1 ? 0.0f : 1.0f);
			//	vertices.push_back(v);
			//}

			indices.push_back(0);
			indices.push_back(2);
			indices.push_back(1);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(3);

			mShapes[QUAD] = CreateMesh(vertices, indices, 0);

			vertices.clear();
			indices.clear();
		}
    
		//Cube
	{
			indices = { 0, 1, 2, 0, 2, 3, //front
					4, 6, 5, 4, 7, 6, //right
					8, 10, 9, 8, 11, 10, //back
					12, 13, 14, 12, 14, 15, //left
					16, 18, 17, 16, 19, 18, //upper
					20, 21, 22, 20, 22, 23 }; //bottom

		//front
		Vertex v;
		v.tangent = glm::vec3(0.0f, 1.0f, 0.0f);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.tex = glm::vec2(0.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.tex = glm::vec2(1.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.tex = glm::vec2(1.0f, 1.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		v.tex = glm::vec2(0.0f, 1.0f);
		vertices.push_back(v);

		//right
		v.tangent = glm::vec3(0.0f, 1.0f, 0.0f);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 1.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 1.0f);
		vertices.push_back(v);

		//back
		v.tangent = glm::vec3(0.0f, -1.0f, 0.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		v.tex = glm::vec2(1.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		v.tex = glm::vec2(0.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		v.tex = glm::vec2(0.0f, 1.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		v.tex = glm::vec2(1.0f, 1.0f);
		vertices.push_back(v);

		//left
		v.tangent = glm::vec3(0.0f, 0.0f, -1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 1.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 1.0f);
		vertices.push_back(v);

		//upper
		v.tangent = glm::vec3(0.0f, 0.0f, 1.0f);
		v.position = glm::vec3(0.5f, 0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, 0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 1.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, 0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 1.0f);
		vertices.push_back(v);

		//bottom
		v.tangent = glm::vec3(0.0f, 0.0f, -1.0f);
		v.position = glm::vec3(-0.5f, -0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, -0.5f);
		v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 0.0f);
		vertices.push_back(v);
		v.position = glm::vec3(0.5f, -0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		v.tex = glm::vec2(1.0f, 1.0f);
		vertices.push_back(v);
		v.position = glm::vec3(-0.5f, -0.5f, 0.5f);
		v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		v.tex = glm::vec2(0.0f, 1.0f);
		vertices.push_back(v);

		mShapes[CUBE] = CreateMesh(vertices, indices);

		vertices.clear();
		indices.clear();
		}
		//Sphere
		const float PI = 3.141592f;
		int n = 32;
		float d = 2.0f * PI / float(n * 2);
		for (int i = 0; i <= n * 2; i++)
		{
			float s = i * 2.0f * PI / float(n * 2);
			for (int j = 0; j <= n; j++)
			{
				float t = j * PI / float(n);
				float x = 0.5f*cos(s) * sin(t);
				float y = 0.5f*sin(s) * sin(t);
				float z = 0.5f*cos(t);
				// Create vertex
				Vertex v;
				v.position = glm::vec3(x, y, z);
				v.normal = glm::vec3(x, y, z);
				v.tex = glm::vec2(s / (2.0f * PI), t / PI);
				v.tangent = glm::vec3(-sinf(s), cosf(s), 0.0);

				vertices.push_back(v);

				// Add indices
				if (i > 0 && j > 0)
				{
					// i, j, k
					indices.push_back((i - 1) * (n + 1) + (j - 1));
					indices.push_back((i - 1) * (n + 1) + j);
					indices.push_back((i) * (n + 1) + j);
					// i, k, l
					indices.push_back((i - 1) * (n + 1) + (j - 1));
					indices.push_back((i) * (n + 1) + j);
					indices.push_back((i) * (n + 1) + (j - 1));
				}
			}
		}

		mShapes[SPHERE] = CreateMesh(vertices, indices);

		vertices.clear();
		indices.clear();

		//Teapot
		// Created with n x n grid of quads
		n = 12;

		static unsigned int TeapotIndex[][16] = {
		1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
		4, 17, 18, 19,  8, 20, 21, 22, 12, 23, 24, 25, 16, 26, 27, 28,
		19, 29, 30, 31, 22, 32, 33, 34, 25, 35, 36, 37, 28, 38, 39, 40,
		31, 41, 42,  1, 34, 43, 44,  5, 37, 45, 46,  9, 40, 47, 48, 13,
		13, 14, 15, 16, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
		16, 26, 27, 28, 52, 61, 62, 63, 56, 64, 65, 66, 60, 67, 68, 69,
		28, 38, 39, 40, 63, 70, 71, 72, 66, 73, 74, 75, 69, 76, 77, 78,
		40, 47, 48, 13, 72, 79, 80, 49, 75, 81, 82, 53, 78, 83, 84, 57,
		57, 58, 59, 60, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
		60, 67, 68, 69, 88, 97, 98, 99, 92,100,101,102, 96,103,104,105,
		69, 76, 77, 78, 99,106,107,108,102,109,110,111,105,112,113,114,
		78, 83, 84, 57,108,115,116, 85,111,117,118, 89,114,119,120, 93,
		121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,
		124,137,138,121,128,139,140,125,132,141,142,129,136,143,144,133,
		133,134,135,136,145,146,147,148,149,150,151,152, 69,153,154,155,
		136,143,144,133,148,156,157,145,152,158,159,149,155,160,161, 69,
		162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,
		165,178,179,162,169,180,181,166,173,182,183,170,177,184,185,174,
		174,175,176,177,186,187,188,189,190,191,192,193,194,195,196,197,
		177,184,185,174,189,198,199,186,193,200,201,190,197,202,203,194,
		204,204,204,204,207,208,209,210,211,211,211,211,212,213,214,215,
		204,204,204,204,210,217,218,219,211,211,211,211,215,220,221,222,
		204,204,204,204,219,224,225,226,211,211,211,211,222,227,228,229,
		204,204,204,204,226,230,231,207,211,211,211,211,229,232,233,212,
		212,213,214,215,234,235,236,237,238,239,240,241,242,243,244,245,
		215,220,221,222,237,246,247,248,241,249,250,251,245,252,253,254,
		222,227,228,229,248,255,256,257,251,258,259,260,254,261,262,263,
		229,232,233,212,257,264,265,234,260,266,267,238,263,268,269,242,
		270,270,270,270,279,280,281,282,275,276,277,278,271,272,273,274,
		270,270,270,270,282,289,290,291,278,286,287,288,274,283,284,285,
		270,270,270,270,291,298,299,300,288,295,296,297,285,292,293,294,
		270,270,270,270,300,305,306,279,297,303,304,275,294,301,302,271 };

		static glm::vec3 TeapotPoints[] = {
			glm::vec3(1.4,0.0,2.4), glm::vec3(1.4,-0.784,2.4), glm::vec3(0.784,-1.4,2.4),
			glm::vec3(0.0,-1.4,2.4), glm::vec3(1.3375,0.0,2.53125),
			glm::vec3(1.3375,-0.749,2.53125), glm::vec3(0.749,-1.3375,2.53125),
			glm::vec3(0.0,-1.3375,2.53125), glm::vec3(1.4375,0.0,2.53125),
			glm::vec3(1.4375,-0.805,2.53125), glm::vec3(0.805,-1.4375,2.53125),
			glm::vec3(0.0,-1.4375,2.53125), glm::vec3(1.5,0.0,2.4), glm::vec3(1.5,-0.84,2.4),
			glm::vec3(0.84,-1.5,2.4), glm::vec3(0.0,-1.5,2.4), glm::vec3(-0.784,-1.4,2.4),
			glm::vec3(-1.4,-0.784,2.4), glm::vec3(-1.4,0.0,2.4),
			glm::vec3(-0.749,-1.3375,2.53125), glm::vec3(-1.3375,-0.749,2.53125),
			glm::vec3(-1.3375,0.0,2.53125), glm::vec3(-0.805,-1.4375,2.53125),
			glm::vec3(-1.4375,-0.805,2.53125), glm::vec3(-1.4375,0.0,2.53125),
			glm::vec3(-0.84,-1.5,2.4), glm::vec3(-1.5,-0.84,2.4), glm::vec3(-1.5,0.0,2.4),
			glm::vec3(-1.4,0.784,2.4), glm::vec3(-0.784,1.4,2.4), glm::vec3(0.0,1.4,2.4),
			glm::vec3(-1.3375,0.749,2.53125), glm::vec3(-0.749,1.3375,2.53125),
			glm::vec3(0.0,1.3375,2.53125), glm::vec3(-1.4375,0.805,2.53125),
			glm::vec3(-0.805,1.4375,2.53125), glm::vec3(0.0,1.4375,2.53125),
			glm::vec3(-1.5,0.84,2.4), glm::vec3(-0.84,1.5,2.4), glm::vec3(0.0,1.5,2.4),
			glm::vec3(0.784,1.4,2.4), glm::vec3(1.4,0.784,2.4), glm::vec3(0.749,1.3375,2.53125),
			glm::vec3(1.3375,0.749,2.53125), glm::vec3(0.805,1.4375,2.53125),
			glm::vec3(1.4375,0.805,2.53125), glm::vec3(0.84,1.5,2.4), glm::vec3(1.5,0.84,2.4),
			glm::vec3(1.75,0.0,1.875), glm::vec3(1.75,-0.98,1.875), glm::vec3(0.98,-1.75,1.875),
			glm::vec3(0.0,-1.75,1.875), glm::vec3(2.0,0.0,1.35), glm::vec3(2.0,-1.12,1.35),
			glm::vec3(1.12,-2.0,1.35), glm::vec3(0.0,-2.0,1.35), glm::vec3(2.0,0.0,0.9),
			glm::vec3(2.0,-1.12,0.9), glm::vec3(1.12,-2.0,0.9), glm::vec3(0.0,-2.0,0.9),
			glm::vec3(-0.98,-1.75,1.875), glm::vec3(-1.75,-0.98,1.875),
			glm::vec3(-1.75,0.0,1.875), glm::vec3(-1.12,-2.0,1.35), glm::vec3(-2.0,-1.12,1.35),
			glm::vec3(-2.0,0.0,1.35), glm::vec3(-1.12,-2.0,0.9), glm::vec3(-2.0,-1.12,0.9),
			glm::vec3(-2.0,0.0,0.9), glm::vec3(-1.75,0.98,1.875), glm::vec3(-0.98,1.75,1.875),
			glm::vec3(0.0,1.75,1.875), glm::vec3(-2.0,1.12,1.35), glm::vec3(-1.12,2.0,1.35),
			glm::vec3(0.0,2.0,1.35), glm::vec3(-2.0,1.12,0.9), glm::vec3(-1.12,2.0,0.9),
			glm::vec3(0.0,2.0,0.9), glm::vec3(0.98,1.75,1.875), glm::vec3(1.75,0.98,1.875),
			glm::vec3(1.12,2.0,1.35), glm::vec3(2.0,1.12,1.35), glm::vec3(1.12,2.0,0.9),
			glm::vec3(2.0,1.12,0.9), glm::vec3(2.0,0.0,0.45), glm::vec3(2.0,-1.12,0.45),
			glm::vec3(1.12,-2.0,0.45), glm::vec3(0.0,-2.0,0.45), glm::vec3(1.5,0.0,0.225),
			glm::vec3(1.5,-0.84,0.225), glm::vec3(0.84,-1.5,0.225), glm::vec3(0.0,-1.5,0.225),
			glm::vec3(1.5,0.0,0.15), glm::vec3(1.5,-0.84,0.15), glm::vec3(0.84,-1.5,0.15),
			glm::vec3(0.0,-1.5,0.15), glm::vec3(-1.12,-2.0,0.45), glm::vec3(-2.0,-1.12,0.45),
			glm::vec3(-2.0,0.0,0.45), glm::vec3(-0.84,-1.5,0.225), glm::vec3(-1.5,-0.84,0.225),
			glm::vec3(-1.5,0.0,0.225), glm::vec3(-0.84,-1.5,0.15), glm::vec3(-1.5,-0.84,0.15),
			glm::vec3(-1.5,0.0,0.15), glm::vec3(-2.0,1.12,0.45), glm::vec3(-1.12,2.0,0.45),
			glm::vec3(0.0,2.0,0.45), glm::vec3(-1.5,0.84,0.225), glm::vec3(-0.84,1.5,0.225),
			glm::vec3(0.0,1.5,0.225), glm::vec3(-1.5,0.84,0.15), glm::vec3(-0.84,1.5,0.15),
			glm::vec3(0.0,1.5,0.15), glm::vec3(1.12,2.0,0.45), glm::vec3(2.0,1.12,0.45),
			glm::vec3(0.84,1.5,0.225), glm::vec3(1.5,0.84,0.225), glm::vec3(0.84,1.5,0.15),
			glm::vec3(1.5,0.84,0.15), glm::vec3(-1.6,0.0,2.025), glm::vec3(-1.6,-0.3,2.025),
			glm::vec3(-1.5,-0.3,2.25), glm::vec3(-1.5,0.0,2.25), glm::vec3(-2.3,0.0,2.025),
			glm::vec3(-2.3,-0.3,2.025), glm::vec3(-2.5,-0.3,2.25), glm::vec3(-2.5,0.0,2.25),
			glm::vec3(-2.7,0.0,2.025), glm::vec3(-2.7,-0.3,2.025), glm::vec3(-3.0,-0.3,2.25),
			glm::vec3(-3.0,0.0,2.25), glm::vec3(-2.7,0.0,1.8), glm::vec3(-2.7,-0.3,1.8),
			glm::vec3(-3.0,-0.3,1.8), glm::vec3(-3.0,0.0,1.8), glm::vec3(-1.5,0.3,2.25),
			glm::vec3(-1.6,0.3,2.025), glm::vec3(-2.5,0.3,2.25), glm::vec3(-2.3,0.3,2.025),
			glm::vec3(-3.0,0.3,2.25), glm::vec3(-2.7,0.3,2.025), glm::vec3(-3.0,0.3,1.8),
			glm::vec3(-2.7,0.3,1.8), glm::vec3(-2.7,0.0,1.575), glm::vec3(-2.7,-0.3,1.575),
			glm::vec3(-3.0,-0.3,1.35), glm::vec3(-3.0,0.0,1.35), glm::vec3(-2.5,0.0,1.125),
			glm::vec3(-2.5,-0.3,1.125), glm::vec3(-2.65,-0.3,0.9375),
			glm::vec3(-2.65,0.0,0.9375), glm::vec3(-2.0,-0.3,0.9), glm::vec3(-1.9,-0.3,0.6),
			glm::vec3(-1.9,0.0,0.6), glm::vec3(-3.0,0.3,1.35), glm::vec3(-2.7,0.3,1.575),
			glm::vec3(-2.65,0.3,0.9375), glm::vec3(-2.5,0.3,1.125), glm::vec3(-1.9,0.3,0.6),
			glm::vec3(-2.0,0.3,0.9), glm::vec3(1.7,0.0,1.425), glm::vec3(1.7,-0.66,1.425),
			glm::vec3(1.7,-0.66,0.6), glm::vec3(1.7,0.0,0.6), glm::vec3(2.6,0.0,1.425),
			glm::vec3(2.6,-0.66,1.425), glm::vec3(3.1,-0.66,0.825), glm::vec3(3.1,0.0,0.825),
			glm::vec3(2.3,0.0,2.1), glm::vec3(2.3,-0.25,2.1), glm::vec3(2.4,-0.25,2.025),
			glm::vec3(2.4,0.0,2.025), glm::vec3(2.7,0.0,2.4), glm::vec3(2.7,-0.25,2.4),
			glm::vec3(3.3,-0.25,2.4), glm::vec3(3.3,0.0,2.4), glm::vec3(1.7,0.66,0.6),
			glm::vec3(1.7,0.66,1.425), glm::vec3(3.1,0.66,0.825), glm::vec3(2.6,0.66,1.425),
			glm::vec3(2.4,0.25,2.025), glm::vec3(2.3,0.25,2.1), glm::vec3(3.3,0.25,2.4),
			glm::vec3(2.7,0.25,2.4), glm::vec3(2.8,0.0,2.475), glm::vec3(2.8,-0.25,2.475),
			glm::vec3(3.525,-0.25,2.49375), glm::vec3(3.525,0.0,2.49375),
			glm::vec3(2.9,0.0,2.475), glm::vec3(2.9,-0.15,2.475), glm::vec3(3.45,-0.15,2.5125),
			glm::vec3(3.45,0.0,2.5125), glm::vec3(2.8,0.0,2.4), glm::vec3(2.8,-0.15,2.4),
			glm::vec3(3.2,-0.15,2.4), glm::vec3(3.2,0.0,2.4), glm::vec3(3.525,0.25,2.49375),
			glm::vec3(2.8,0.25,2.475), glm::vec3(3.45,0.15,2.5125), glm::vec3(2.9,0.15,2.475),
			glm::vec3(3.2,0.15,2.4), glm::vec3(2.8,0.15,2.4), glm::vec3(0.0,0.0,3.15),
			glm::vec3(0.0,-0.002,3.15), glm::vec3(0.002,0.0,3.15), glm::vec3(0.8,0.0,3.15),
			glm::vec3(0.8,-0.45,3.15), glm::vec3(0.45,-0.8,3.15), glm::vec3(0.0,-0.8,3.15),
			glm::vec3(0.0,0.0,2.85), glm::vec3(0.2,0.0,2.7), glm::vec3(0.2,-0.112,2.7),
			glm::vec3(0.112,-0.2,2.7), glm::vec3(0.0,-0.2,2.7), glm::vec3(-0.002,0.0,3.15),
			glm::vec3(-0.45,-0.8,3.15), glm::vec3(-0.8,-0.45,3.15), glm::vec3(-0.8,0.0,3.15),
			glm::vec3(-0.112,-0.2,2.7), glm::vec3(-0.2,-0.112,2.7), glm::vec3(-0.2,0.0,2.7),
			glm::vec3(0.0,0.002,3.15), glm::vec3(-0.8,0.45,3.15), glm::vec3(-0.45,0.8,3.15),
			glm::vec3(0.0,0.8,3.15), glm::vec3(-0.2,0.112,2.7), glm::vec3(-0.112,0.2,2.7),
			glm::vec3(0.0,0.2,2.7), glm::vec3(0.45,0.8,3.15), glm::vec3(0.8,0.45,3.15),
			glm::vec3(0.112,0.2,2.7), glm::vec3(0.2,0.112,2.7), glm::vec3(0.4,0.0,2.55),
			glm::vec3(0.4,-0.224,2.55), glm::vec3(0.224,-0.4,2.55), glm::vec3(0.0,-0.4,2.55),
			glm::vec3(1.3,0.0,2.55), glm::vec3(1.3,-0.728,2.55), glm::vec3(0.728,-1.3,2.55),
			glm::vec3(0.0,-1.3,2.55), glm::vec3(1.3,0.0,2.4), glm::vec3(1.3,-0.728,2.4),
			glm::vec3(0.728,-1.3,2.4), glm::vec3(0.0,-1.3,2.4), glm::vec3(-0.224,-0.4,2.55),
			glm::vec3(-0.4,-0.224,2.55), glm::vec3(-0.4,0.0,2.55), glm::vec3(-0.728,-1.3,2.55),
			glm::vec3(-1.3,-0.728,2.55), glm::vec3(-1.3,0.0,2.55), glm::vec3(-0.728,-1.3,2.4),
			glm::vec3(-1.3,-0.728,2.4), glm::vec3(-1.3,0.0,2.4), glm::vec3(-0.4,0.224,2.55),
			glm::vec3(-0.224,0.4,2.55), glm::vec3(0.0,0.4,2.55), glm::vec3(-1.3,0.728,2.55),
			glm::vec3(-0.728,1.3,2.55), glm::vec3(0.0,1.3,2.55), glm::vec3(-1.3,0.728,2.4),
			glm::vec3(-0.728,1.3,2.4), glm::vec3(0.0,1.3,2.4), glm::vec3(0.224,0.4,2.55),
			glm::vec3(0.4,0.224,2.55), glm::vec3(0.728,1.3,2.55), glm::vec3(1.3,0.728,2.55),
			glm::vec3(0.728,1.3,2.4), glm::vec3(1.3,0.728,2.4), glm::vec3(0.0,0.0,0.0),
			glm::vec3(1.5,0.0,0.15), glm::vec3(1.5,0.84,0.15), glm::vec3(0.84,1.5,0.15),
			glm::vec3(0.0,1.5,0.15), glm::vec3(1.5,0.0,0.075), glm::vec3(1.5,0.84,0.075),
			glm::vec3(0.84,1.5,0.075), glm::vec3(0.0,1.5,0.075), glm::vec3(1.425,0.0,0.0),
			glm::vec3(1.425,0.798,0.0), glm::vec3(0.798,1.425,0.0), glm::vec3(0.0,1.425,0.0),
			glm::vec3(-0.84,1.5,0.15), glm::vec3(-1.5,0.84,0.15), glm::vec3(-1.5,0.0,0.15),
			glm::vec3(-0.84,1.5,0.075), glm::vec3(-1.5,0.84,0.075), glm::vec3(-1.5,0.0,0.075),
			glm::vec3(-0.798,1.425,0.0), glm::vec3(-1.425,0.798,0.0), glm::vec3(-1.425,0.0,0.0),
			glm::vec3(-1.5,-0.84,0.15), glm::vec3(-0.84,-1.5,0.15), glm::vec3(0.0,-1.5,0.15),
			glm::vec3(-1.5,-0.84,0.075), glm::vec3(-0.84,-1.5,0.075), glm::vec3(0.0,-1.5,0.075),
			glm::vec3(-1.425,-0.798,0.0), glm::vec3(-0.798,-1.425,0.0),
			glm::vec3(0.0,-1.425,0.0), glm::vec3(0.84,-1.5,0.15), glm::vec3(1.5,-0.84,0.15),
			glm::vec3(0.84,-1.5,0.075), glm::vec3(1.5,-0.84,0.075), glm::vec3(0.798,-1.425,0.0),
			glm::vec3(1.425,-0.798,0.0) };


		int npatches = sizeof(TeapotIndex) / sizeof(TeapotIndex[0]);
		const int nv = npatches * (n + 1) * (n + 1);
		int nq = npatches * n * n;

		for (int p = 0; p < npatches; ++p) // For each patch
		{
			for (int i = 0; i <= n; ++i) // Grid in u direction
			{
				float u = float(i) / n;
				for (int j = 0; j <= n; ++j) // Grid in v direction
				{
					float v = float(j) / n;

					// Four u weights
					float u0 = (1.0f - u) * (1.0f - u) * (1.0f - u);
					float u1 = 3.0f * (1.0f - u) * (1.0f - u) * u;
					float u2 = 3.0f * (1.0f - u) * u * u;
					float u3 = u * u * u;

					// Three du weights
					float du0 = (1.0f - u) * (1.0f - u);
					float du1 = 2.0f * (1.0f - u) * u;
					float du2 = u * u;

					// Four v weights
					float v0 = (1.0f - v) * (1.0f - v) * (1.0f - v);
					float v1 = 3.0f * (1.0f - v) * (1.0f - v) * v;
					float v2 = 3.0f * (1.0f - v) * v * v;
					float v3 = v * v * v;

					// Three dv weights
					float dv0 = (1.0f - v) * (1.0f - v);
					float dv1 = 2.0f * (1.0f - v) * v;
					float dv2 = v * v;

					// Grab the 16 control points for Bezier patch.
					glm::vec3* p00 = &TeapotPoints[TeapotIndex[p][0] - 1];
					glm::vec3* p01 = &TeapotPoints[TeapotIndex[p][1] - 1];
					glm::vec3* p02 = &TeapotPoints[TeapotIndex[p][2] - 1];
					glm::vec3* p03 = &TeapotPoints[TeapotIndex[p][3] - 1];
					glm::vec3* p10 = &TeapotPoints[TeapotIndex[p][4] - 1];
					glm::vec3* p11 = &TeapotPoints[TeapotIndex[p][5] - 1];
					glm::vec3* p12 = &TeapotPoints[TeapotIndex[p][6] - 1];
					glm::vec3* p13 = &TeapotPoints[TeapotIndex[p][7] - 1];
					glm::vec3* p20 = &TeapotPoints[TeapotIndex[p][8] - 1];
					glm::vec3* p21 = &TeapotPoints[TeapotIndex[p][9] - 1];
					glm::vec3* p22 = &TeapotPoints[TeapotIndex[p][10] - 1];
					glm::vec3* p23 = &TeapotPoints[TeapotIndex[p][11] - 1];
					glm::vec3* p30 = &TeapotPoints[TeapotIndex[p][12] - 1];
					glm::vec3* p31 = &TeapotPoints[TeapotIndex[p][13] - 1];
					glm::vec3* p32 = &TeapotPoints[TeapotIndex[p][14] - 1];
					glm::vec3* p33 = &TeapotPoints[TeapotIndex[p][15] - 1];

					// Evaluate the Bezier patch at (u,v)
					glm::vec3 V =
						u0 * v0 * (*p00) + u0 * v1 * (*p01) + u0 * v2 * (*p02) + u0 * v3 * (*p03) +
						u1 * v0 * (*p10) + u1 * v1 * (*p11) + u1 * v2 * (*p12) + u1 * v3 * (*p13) +
						u2 * v0 * (*p20) + u2 * v1 * (*p21) + u2 * v2 * (*p22) + u2 * v3 * (*p23) +
						u3 * v0 * (*p30) + u3 * v1 * (*p31) + u3 * v2 * (*p32) + u3 * v3 * (*p33);

					// Create vertex
					Vertex vert;
					vert.position = glm::vec3(V[0], V[1], V[2]);

					// Evaluate the u-tangent of the Bezier patch at (u,v)
					glm::vec3 du =
						du0 * v0 * (*p10 - *p00) + du0 * v1 * (*p11 - *p01) + du0 * v2 * (*p12 - *p02) + du0 * v3 * (*p13 - *p03) +
						du1 * v0 * (*p20 - *p10) + du1 * v1 * (*p21 - *p11) + du1 * v2 * (*p22 - *p12) + du1 * v3 * (*p23 - *p13) +
						du2 * v0 * (*p30 - *p20) + du2 * v1 * (*p31 - *p21) + du2 * v2 * (*p32 - *p22) + du2 * v3 * (*p33 - *p23);
					vert.tangent = du;

					// Evaluate the v-tangent of the Bezier patch at (u,v)
					glm::vec3 dv =
						u0 * dv0 * (*p01 - *p00) + u0 * dv1 * (*p02 - *p01) + u0 * dv2 * (*p03 - *p02) +
						u1 * dv0 * (*p11 - *p10) + u1 * dv1 * (*p12 - *p11) + u1 * dv2 * (*p13 - *p12) +
						u2 * dv0 * (*p21 - *p20) + u2 * dv1 * (*p22 - *p21) + u2 * dv2 * (*p23 - *p22) +
						u3 * dv0 * (*p31 - *p30) + u3 * dv1 * (*p32 - *p31) + u3 * dv2 * (*p33 - *p32);

					// Calculate the surface normal as the cross product of the two tangents.
					vert.normal = glm::vec3(glm::cross(dv, du));

					// Get texture coordinates
					vert.tex = glm::vec2(u, v);
					vertices.push_back(vert);

					// Create a quad for all but the first edge vertices
					if (i > 0 && j > 0)
					{
						indices.push_back(p * (n + 1) * (n + 1) + (i - 1) * (n + 1) + (j - 1));
						indices.push_back(p * (n + 1) * (n + 1) + (i - 1) * (n + 1) + (j));
						indices.push_back(p * (n + 1) * (n + 1) + (i) * (n + 1) + (j));

						indices.push_back(p * (n + 1) * (n + 1) + (i - 1) * (n + 1) + (j - 1));
						indices.push_back(p * (n + 1) * (n + 1) + (i) * (n + 1) + (j));
						indices.push_back(p * (n + 1) * (n + 1) + (i) * (n + 1) + (j - 1));
					}
				}
			}
		}

		mShapes[TEAPOT] = CreateMesh(vertices, indices);
	}

	Mesh* ResourceManager::CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex, VertexBuffer* vbo)
	{
		VertexArray* VAO = new VertexArray();
		ElementArrayBuffer* EBO = new ElementArrayBuffer();
		VertexBuffer* VBO = new VertexBuffer();

		VAO->Bind();

		// Send vertex information to VBO
		VBO->AddData(&vertices[0], vertices.size(), sizeof(Vertex));

		// Set up index buffer EBO
		EBO->AddData(&indices[0], indices.size(), sizeof(unsigned int));

		// Position
		VAO->Push(3, GL_FLOAT, sizeof(float));
		// Normal
		VAO->Push(3, GL_FLOAT, sizeof(float));
		// Texture coordinates
		VAO->Push(2, GL_FLOAT, sizeof(float));
		// Tangent
		VAO->Push(3, GL_FLOAT, sizeof(float));
		VAO->AddLayout();

		if (vbo)
		{
			vbo->Bind();
			VAO->Push(4, GL_INT, sizeof(int));
			VAO->Push(4, GL_FLOAT, sizeof(float));
			VAO->AddLayout();
		}

		// Unbind VAO
		VAO->Unbind();

		Mesh* mesh = new Mesh();

		mesh->mpVAO = VAO;
		mesh->mpEBO = EBO;
		mesh->mpVBO = VBO;
		mesh->mMaterialIndex = materialIndex;

		return mesh;
	}
}
