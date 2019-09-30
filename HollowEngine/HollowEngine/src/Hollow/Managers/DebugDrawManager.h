#pragma once

namespace Hollow
{
	class HOLLOW_API DebugDrawManager
	{
		SINGLETON(DebugDrawManager)
	public:
		void Init();
		void CleanUp();

		void DebugLine();
		void DebugCircle();
		void DebugAxes();
		void DebugSphere(glm::vec3 position, float scale);
		void DebugCube();
	};
}