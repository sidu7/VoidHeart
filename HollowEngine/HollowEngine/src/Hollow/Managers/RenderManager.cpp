#include <hollowpch.h>
#include "RenderManager.h"

#include "../GameWindow.h"

namespace Hollow {

	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Init(GameWindow* pWindow)
	{
		// Init opengl
		if (glewInit() != GLEW_OK)
		{
			HW_CORE_ERROR("Failed to initialize GLEW");
		}

		mpWindow = pWindow;
	}

	void RenderManager::Update()
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		SDL_GL_SwapWindow(mpWindow->GetWindow());
	}
}