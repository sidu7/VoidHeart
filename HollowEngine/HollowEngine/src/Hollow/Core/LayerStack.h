#pragma once

#include "Hollow/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hollow {

	class LayerStack
	{
	public:
		HOLLOW_API LayerStack();
		HOLLOW_API ~LayerStack();
		 
		HOLLOW_API void PushLayer(Layer* layer);
		HOLLOW_API void PushOverlay(Layer* overlay);
		HOLLOW_API void PopLayer(Layer* layer);
		HOLLOW_API void PopOverlay(Layer* overlay);
		 
		HOLLOW_API std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
		HOLLOW_API std::vector<Layer*>::iterator end() { return mLayers.end(); }
	private:
		std::vector<Layer*> mLayers;
		unsigned int mLayerInsertIndex = 0;
	};

}