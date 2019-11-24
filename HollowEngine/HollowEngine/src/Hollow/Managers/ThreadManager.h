#pragma once
#include "Hollow/Common.h"
#define MAX_THREADS 2

namespace Hollow
{
#define THREAD_FUNCTION(x,y,z) std::bind(&x,this,y,z)
	
	class ThreadManager
	{
		SINGLETON(ThreadManager);
	public:
		HOLLOW_API  ~ThreadManager() {}

		HOLLOW_API  void Init();
		HOLLOW_API  void CleanUp();
		
		// Takes only functions with start and end index 
		template<typename FunctionType>
		HOLLOW_API  std::future<void> Push(const FunctionType& function)
		{
			std::packaged_task<void(unsigned int, unsigned int)> task(std::move(function));
			std::future<void> res(task.get_future());
			mTaskQueue.Push(std::move(task));
			return res;
		}

		HOLLOW_API  size_t GetThreadCount() const { return mThreads.size(); }

	private:
		void WaitForTask();

	private:
		bool mIsRunning;
		ThreadSafeQueue<std::packaged_task<void(unsigned int, unsigned int)>> mTaskQueue;
		std::vector<std::thread> mThreads;
	};
}