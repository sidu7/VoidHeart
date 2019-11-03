#pragma once

#define MAX_THREADS 4

namespace Hollow
{
#define THREAD_FUNCTION(x,y,z) std::bind(&x,this,y,z)
	
	class HOLLOW_API ThreadManager
	{
		SINGLETON(ThreadManager);
	public:
		~ThreadManager() {}

		void Init();
		void CleanUp();
		
		// Takes only functions with start and end index 
		template<typename FunctionType>
		std::future<void> Push(const FunctionType& function)
		{
			std::packaged_task<void(unsigned int, unsigned int)> task(std::move(function));
			std::future<void> res(task.get_future());
			mTaskQueue.Push(std::move(task));
			return res;
		}

		unsigned int GetThreadCount() const { return mThreads.size(); }

	private:
		void WaitForTask();

	private:
		bool mIsRunning;
		ThreadSafeQueue<std::packaged_task<void(unsigned int, unsigned int)>> mTaskQueue;
		std::vector<std::thread> mThreads;
	};
}