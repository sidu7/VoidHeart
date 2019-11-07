#pragma once


namespace Hollow
{
	const unsigned int Animation_Flags = aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_OptimizeMeshes |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices;

	// May need to add FlipUVs flag
	const unsigned int Model_Flags = aiProcess_RemoveRedundantMaterials |
		aiProcess_Triangulate |	aiProcess_OptimizeMeshes | aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_CalcTangentSpace;

	struct AnimationData
	{
		std::vector<std::pair<double, glm::vec3>> mKeyPositions;
		std::vector<std::pair<double, glm::quat>> mKeyRotations;
		std::vector<std::pair<double, glm::vec3>> mKeyScalings;
		double mDuration;
		double mTicksPerSec;
	};
}
