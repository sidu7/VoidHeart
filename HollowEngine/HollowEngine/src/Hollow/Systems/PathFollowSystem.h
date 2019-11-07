#pragma once
#include "System.h"

namespace Hollow
{
	class PathFollow;
	
	class PathFollowSystem : public System
	{
		REGISTERSYSTEM(PathFollowSystem, 1);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void AddGameObject(GameObject* pGameObject) override;
		HOLLOW_API void Update() override;

	private:
		void CalculateControlPointMatrices(PathFollow* pathFollow);
		void CreateArcLengthTable(PathFollow* pathFollow);
		void CreateDebugVAO(PathFollow* pathFollow);
		void StopPathMotion();
		void StartPathMotion();
		inline std::pair<float, int> SearchInArcTable(const float distance, PathFollow* pathFollow);
		int BinarySearch(unsigned int start, unsigned int end, float distance, std::vector<std::pair<float, std::pair<float, int>>>& list);

	private:
		bool StopMoving;
	};
}