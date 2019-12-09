#pragma once

#include "Hollow/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hollow {

	/// <summary>
	/// The functionality of the full class is not yet implemented.
	/// </summary>
	class LayerStack
	{
	public:
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API LayerStack();
		
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API ~LayerStack();
		 
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API void PushLayer(Layer* layer);

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API void PushOverlay(Layer* overlay);

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API void PopLayer(Layer* layer);

		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API void PopOverlay(Layer* overlay);
		 
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
		
		/// <summary>
		/// Not Yet Implemented
		/// </summary>
		HOLLOW_API std::vector<Layer*>::iterator end() { return mLayers.end(); }
	private:
		std::vector<Layer*> mLayers;
		unsigned int mLayerInsertIndex = 0;
	};

}