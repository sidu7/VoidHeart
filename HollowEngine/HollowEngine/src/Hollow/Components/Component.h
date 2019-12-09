#pragma once
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Common.h"

namespace Hollow
{
	/// <summary>
	/// The base Class for all the components.
	/// </summary>
	class Component
	{
		friend class MemoryManager;
	public:
		/// <inheritdoc>
		/// <summary>
		/// Initializes a new instance of the <see cref="Component"/> class.
		/// </summary>
		/// <param name="name">The name of the component.</param>
		/// <inheritdoc/>
		Component(std::string name) {
			mComponentName = name;
		}
		/// <summary>
		/// Initializes a new instance of the <see cref="Component"/> class and registers it in the Memory Manager.
		/// </summary>
		/// <param name="name">The name of the component.</param>
		/// <param name="component">The pointer to the component to be registered.</param>
		Component(std::string name, Component* component) {
			MemoryManager::Instance().RegisterComponent(name, component);
			mComponentName = name;
		}

		/// <summary>
		/// Initializes the component.
		/// </summary>
		HOLLOW_API virtual void Init() = 0;

		/// <summary>
		/// Gets the index for the component to be put in GameObject/Component Map.
		/// </summary>
		/// <returns>std.type_index. of the component</returns>
		HOLLOW_API virtual std::type_index GetIndex() = 0;

		/// <summary>
		/// Serializes the specified data of the component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API virtual void Serialize(rapidjson::Value::Object data) = 0;

		/// <summary>
		/// Clears the component.
		/// </summary>
		HOLLOW_API virtual void Clear() = 0;

		/// <summary>
		/// Destructor for the instance of the <see cref="Component"/> class.
		/// </summary>
		HOLLOW_API virtual ~Component() {}

		/// <summary>
		/// To set the Debug Display for the Components.
		/// </summary>
		HOLLOW_API virtual void DebugDisplay() = 0;

		/// <summary>
		/// DeSerialize the current component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API virtual void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) = 0;

		/// <summary>
		/// Shows the debug of the Component in the IMGUI.
		/// </summary>
		HOLLOW_API void ShowDebug()
		{
			if (ImGui::TreeNode(mComponentName.c_str()))
			{
				DebugDisplay();
				ImGui::TreePop();
			}
		}
		std::string mComponentName;

		GameObject* mpOwner;
	protected:
		/// <summary>
		/// Creates the component required.
		/// </summary>
		/// <returns>Hollow.Component *.  The pointer to the component created</returns>
		virtual Component* CreateComponent() = 0;
	};
}