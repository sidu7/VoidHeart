#pragma once
#include "System.h"

namespace Hollow
{
	class PathFollow;
	
	/// <summary>
	/// Class PathFollowSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class PathFollowSystem : public System
	{
		REGISTERSYSTEM(PathFollowSystem, 1);
	public:
		/// <summary>
		/// Initializes the System.
		/// </summary>
		HOLLOW_API void Init();
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		HOLLOW_API void AddGameObject(GameObject* pGameObject) override;
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
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