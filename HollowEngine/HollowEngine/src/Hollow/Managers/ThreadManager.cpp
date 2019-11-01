#include <hollowpch.h>
#include "ThreadManager.h"

namespace Hollow
{
	void ThreadManager::Init()
	{
		mIsRunning = true;
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
			Task task;
			if (mTaskQueue.TryPop(task))
			{
				std::pair<unsigned int, unsigned int> args = task.second;
				task.first(args.first, args.second);				
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}
}