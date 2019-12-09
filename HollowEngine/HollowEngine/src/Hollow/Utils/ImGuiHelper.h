#pragma once
#include <ImGui/imgui.h>
#include "Hollow/Core/Core.h"
#include <string>

namespace Hollow
{
	/// <summary>
	/// Class ImGuiHelper.
	/// </summary>
	class ImGuiHelper
	{
	public:
		/// <summary>
		/// Creates ImGui InputText using label name and the text.
		/// </summary>
		/// <param name="label">The label.</param>
		/// <param name="data">The data.</param>
		HOLLOW_API static void InputText(const char* label, std::string& data)
		{
			char* str = (char*)data.c_str();
			ImGui::InputText(label, str,125);
			data = str;
		}

		/// <summary>
		/// Drops down list.
		/// </summary>
		HOLLOW_API static void DropDownList()
		{
			
		}
	};
}
