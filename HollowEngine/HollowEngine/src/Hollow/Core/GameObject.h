#pragma once
#include "Hollow/Common.h"

namespace Hollow {
	class Component;
	class GameObject
	{
		friend class GameObjectFactory;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="GameObject"/> class.
		/// </summary>
		HOLLOW_API GameObject();

		/// <summary>
		/// Destructor for the instance of the <see cref="GameObject"/> class.
		/// </summary>
		HOLLOW_API ~GameObject();

		template <typename T>
		T* GetComponent();

		/// <summary>
		/// Adds the component passed to the GameObejct/Component map.
		/// Sets the owner of the component as the GameObject and sets the indice for the component.
		/// </summary>
		/// <param name="component">The Component to be added to the GameObject.</param>
		HOLLOW_API void AddComponent(Component* component);

		/// <summary>
		/// Destroys this instance of the GameObject in the Memory Manager to free up the pointer in the memory pool.
		/// </summary>
		HOLLOW_API void Destroy();

		/// <summary>
		/// Debug display for the GameObject components.
		/// </summary>
		HOLLOW_API void DebugDisplay();

		/// <summary>
		/// Deserializes the whole GameObject in a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON.</param>
		HOLLOW_API void Deserialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
	
	public:
		std::unordered_map<std::type_index, Component*> mComponents;
		std::vector<std::type_index> mIndices;
		unsigned int mID;
		int mType;
		bool mActive;
		std::string mTag;
	private:
		static unsigned int ID;
		std::string mDType;
	};

	template <typename T>
	T* GameObject::GetComponent()
	{
		if (mComponents.find(std::type_index(typeid(T))) != mComponents.end())
		{
			return static_cast<T*>(mComponents[std::type_index(typeid(T))]);
		}
		else
		{
			return nullptr;
		}
	}
}