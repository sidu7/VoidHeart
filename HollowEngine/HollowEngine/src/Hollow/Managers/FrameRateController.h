/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#include "hollowpch.h"

namespace Hollow {

	class FrameRateController {

		SINGLETON(FrameRateController)
	public:
		void SetMaxFrameRate(Uint32 MaxFramerate)
		{
			mMaxFrameRate = MaxFramerate;
			if (mMaxFrameRate != 0)
				mNeededTicksPerFrame = 1000 / mMaxFrameRate;
			else
				mNeededTicksPerFrame = 0;
		}


		void Init()
		{
			mTickStart = mTickEnd = mFrameTime = 0;
		}


		void Close()
		{
			
		}

		float GetFrameTime() {
			//HW_CORE_INFO("{0}", mFrameTime * 1.0f / 1000.0f);
			return mFrameTime * 1.0f / 1000.0f;
		}

		void FrameStart()
		{
			mTickStart = SDL_GetTicks();
		}

		void FrameEnd()
		{
			mTickEnd = SDL_GetTicks();
			while ((mTickEnd - mTickStart) < mNeededTicksPerFrame)
			{
				mTickEnd = SDL_GetTicks();
			}
			mFrameTime = mTickEnd - mTickStart;
		}

	private:
		Uint32 mTickEnd = 0, mTickStart = 0, mNeededTicksPerFrame = 0, mFrameTime = 0, mMaxFrameRate = 0;
	};
}