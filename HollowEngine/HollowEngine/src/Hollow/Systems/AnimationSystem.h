#pragma once
#include "System.h"

namespace Hollow
{
	class AnimationSystem : public System
	{
		REGISTERSYSTEM(AnimationSystem, 2);
	public:
		virtual ~AnimationSystem() {}
		void Update();
		void AddGameObject(GameObject* object);

	private:
		template<typename T>
		inline unsigned int FindT2inList(double time, std::vector<std::pair<double, T>> list)
		{
			for (unsigned int i = 0; i < list.size(); ++i)
			{
				if (list[i].first >= time)
				{
					return i;
				}
			}
			return 0;
		}

	private:
		double mRunTime;
	};
}