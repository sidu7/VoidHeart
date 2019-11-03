#pragma once
#include "System.h"

namespace Hollow
{
	class PathFollow;
	
	class HOLLOW_API PathFollowSystem : public System
	{
		REGISTERSYSTEM(PathFollowSystem, 1);
	public:
		void AddGameObject(GameObject* pGameObject) override;
		void Update() override;

	private:
		void CalculateControlPointMatrices(PathFollow* pathFollow);
		void CreateArcLengthTable(PathFollow* pathFollow);
		void CreateDebugVAO(PathFollow* pathFollow);
		inline std::pair<float, int> SearchInArcTable(const float distance, PathFollow* pathFollow);
	};
}