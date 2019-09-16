#pragma once
#include "hollowpch.h"

#include "Core.h"

namespace Hollow {
	class HOLLOW_API Window
	{
	public:
		virtual ~Window() = default;

		static Window* Create(std::string title, int width, int height);
	};
}