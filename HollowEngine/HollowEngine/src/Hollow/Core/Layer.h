#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Events/Event.h"

namespace Hollow {

	class Layer
	{
	public:
		HOLLOW_API Layer(const std::string& name = "Layer");
		HOLLOW_API virtual ~Layer() = default;
		 
		HOLLOW_API virtual void OnAttach() {}
		HOLLOW_API virtual void OnDetach() {}
		HOLLOW_API virtual void OnUpdate(float ts) {}
		HOLLOW_API virtual void OnImGuiRender() {}
		HOLLOW_API virtual void OnEvent(Event& event) {}
		 
		HOLLOW_API inline const std::string& GetName() const { return mDebugName; }
	protected:
		std::string mDebugName;
	};

}