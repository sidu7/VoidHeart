#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Events/Event.h"

namespace Hollow {

	/// <summary>
	/// The functionality of the full class is not yet implemented.
	/// </summary>
	class Layer
	{
	public:
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API Layer(const std::string& name = "Layer");

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API virtual ~Layer() = default;

		/// <summary>
		/// Not Yet Implemented
		/// </summary> 
		HOLLOW_API virtual void OnAttach() {}

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API virtual void OnDetach() {}

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API virtual void OnUpdate(float ts) {}

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API virtual void OnImGuiRender() {}
		
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API virtual void OnEvent(Event& event) {}
		 
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API inline const std::string& GetName() const { return mDebugName; }
	protected:
		std::string mDebugName;
	};

}