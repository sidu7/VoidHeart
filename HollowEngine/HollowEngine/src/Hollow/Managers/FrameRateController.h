/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#include "Hollow/Common.h"

namespace Hollow {

	/// <summary>
	/// Class FrameRate Controller.
	/// </summary>
	class FrameRateController {

		SINGLETON(FrameRateController)
	public:
		/// <summary>
		/// Sets maximum frame rate allowed.
		/// </summary>
		/// <param name="MaxFramerate">Max frame rate value.</param>
		HOLLOW_API void SetMaxFrameRate(Uint64 MaxFramerate)
		{
			mMaxFrameRate = MaxFramerate;
			if (mMaxFrameRate != 0)
				mNeededTicksPerFrame = 1000.0f / mMaxFrameRate;
			else
				mNeededTicksPerFrame = 0.0f;
		}


		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init()
		{
			mTickStart = mTickEnd = 0;
			mFrameTime = 0.0f;
		}
		
		
		/// <summary>
		/// Gets the frame time.
		/// </summary>
		/// <returns>float.</returns>
		HOLLOW_API float GetFrameTime() {
			return static_cast<float>(mFrameTime * 1.0f / 1000.0f);
		}

		/// <summary>
		/// Records frame start time.
		/// </summary>
		HOLLOW_API void FrameStart()
		{
			mTickStart = SDL_GetPerformanceCounter();
		}

		/// <summary>
		/// Records frame end time and calculates frame time.
		/// </summary>
		HOLLOW_API void FrameEnd()
		{
			mTickEnd = SDL_GetPerformanceCounter();
			while (((mTickEnd - mTickStart) * 1000.0f) / SDL_GetPerformanceFrequency() < mNeededTicksPerFrame)
			{
				mTickEnd = SDL_GetPerformanceCounter();
			}			
			mFrameTime = ((mTickEnd - mTickStart) * 1000.0f) / SDL_GetPerformanceFrequency();
		}

	private:
		UINT64 mTickEnd = 0, mTickStart = 0, mMaxFrameRate = 0;
		float  mNeededTicksPerFrame = 0.0f, mFrameTime = 0.0f;
		
	};
}