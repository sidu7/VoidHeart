#pragma once
#include "System.h"

/// <summary>
/// The Hollow namespace.
/// </summary>
namespace Hollow
{
	class Animator;
	
	/// <summary>
	/// Class AnimationSystem.
	/// Inherits from <see cref="System" />
	/// </summary>
	/// <seealso cref="System" />
	class AnimationSystem : public System
	{
		/// <summary>
		/// Registers this System with the SystemManager
		/// </summary>
		REGISTERSYSTEM(AnimationSystem, 3);
	public:
		/// <summary>
		/// Destructor for AnimationSystem
		/// </summary>
		HOLLOW_API ~AnimationSystem() override {}		
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update() override;
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object) override;

	private:
		void Animate(unsigned int start, unsigned int end);
		void CreateSkeleton(Animator* animator);
		glm::vec3 Interpolate(std::pair<unsigned, unsigned>& index, double timeFrame, std::vector<std::pair<double, glm::vec3>>& list);
		glm::quat SInterpolate(std::pair<unsigned, unsigned>& index, double timeFrame, std::vector<std::pair<double, glm::quat>>& list);
		
		template<typename T>
		inline std::pair<unsigned int, unsigned int> BinarySearch(unsigned int start, unsigned int end, double time, const std::vector<std::pair<double, T>>& list)
		{
			unsigned int mid = start + (end - start)/2;
			if (time == list[mid].first || mid == list.size() - 1 || mid == 0)
			{
				return std::make_pair(mid, -1);
			}
			else if (time > list[mid].first && time < list[mid + 1].first)
			{
				return std::make_pair(mid, mid + 1);
			}
			else if (time > list[mid - 1].first && time < list[mid].first)
			{
				return std::make_pair(mid - 1, mid);
			}
			else if (time < list[mid].first)
			{
				return BinarySearch(start, mid - 1, time, list);
			}
			else if (time > list[mid].first)
			{
				return BinarySearch(mid + 1, end, time, list);
			}
			return std::make_pair(-1, -1);
		}

		template<typename T>
		inline std::pair<unsigned int,unsigned int> FindT2inList(double time, const std::vector<std::pair<double, T>>& list)
		{
			return BinarySearch(0, static_cast<unsigned int>(list.size()) - 1, time, list);
		}
	};
}