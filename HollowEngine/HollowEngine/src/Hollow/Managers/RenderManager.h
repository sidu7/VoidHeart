#pragma once

namespace Hollow {

	class GameWindow;

	class RenderManager
	{
		SINGLETON(RenderManager)
	public:
		~RenderManager();

		void Init(GameWindow* pWindow = nullptr);

		void Update();

	private:
		GameWindow* mpWindow;
	};
}