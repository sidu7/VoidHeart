#pragma once

namespace Hollow {

	class GameWindow;

	class RenderManager
	{
	public:
		RenderManager(GameWindow* pWindow = nullptr);
		~RenderManager();

		void Init(GameWindow* pWindow);

		void Update();

	private:
		GameWindow* mpWindow;
	};
}