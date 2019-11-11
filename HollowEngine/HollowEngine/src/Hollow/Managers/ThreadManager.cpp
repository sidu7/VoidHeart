#include <hollowpch.h>
#include "ThreadManager.h"

namespace Hollow
{
	void ThreadManager::Init()
	{
		mIsRunning = true;
		//unsigned int threads_count = std::thread::hardware_concurrency();
		//threads_count = threads_count == 0 ? MAX_THREADS : threads_count;		
		for (unsigned int i = 0; i < MAX_THREADS; ++i)
		{
			mThreads.push_back(std::thread(&ThreadManager::WaitForTask, this));
		}
	}

	void ThreadManager::CleanUp()
	{
		mIsRunning = false;
		for (unsigned int i = 0; i < mThreads.size(); ++i)
		{
			if (mThreads[i].joinable())
			{
				mThreads[i].join();
			}
		}
	}

	void ThreadManager::WaitForTask()
	{
		while (mIsRunning)
		{
			std::packaged_task<void(unsigned int, unsigned int)> task;
			if (mTaskQueue.TryPop(task))
			{
				task(0, 0);
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}
}