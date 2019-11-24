#pragma once
#include <ImGui/imgui.h>
#include "Hollow/Core/Core.h"
#include <string>

namespace Hollow
{
	class ImGuiHelper
	{
	public:
		HOLLOW_API static void InputText(const char* label, std::string& data)
		{
			if(data.size() == 0)
			{
				return;
			}
			char* str = (char*)data.c_str();
			ImGui::InputText(label, str,125);
			data = str;
		}

		HOLLOW_API static void DropDownList()
		{
			
		}
	};
}
