#pragma once

namespace Hollow 
{	
	class Material;
	class Shader;
	class Mesh;

	struct RenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		Material* mpMaterial;
		bool mCastShadow;
		Shader* mpShader;
		bool mIsAnimated;
		std::vector<glm::mat4> mBoneTransforms;
	};
}