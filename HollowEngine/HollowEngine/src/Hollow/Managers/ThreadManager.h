#pragma once

#define MAX_THREADS 4

namespace Hollow
{
	class HOLLOW_API ThreadManager
	{
		typedef std::pair<std::packaged_task<void(unsigned int, unsigned int)>, std::pair<unsigned int, unsigned int>> Task;

		SINGLETON(ThreadManager);
	public:
		~ThreadManager() {}

		void Init();
		void CleanUp();
		
		// Takes only functions with start and end index 
		template<typename FunctionType>
		std::future<void> Push(const FunctionType& function,unsigned int start, unsigned int end)
		{
			std::packaged_task<void(unsigned int, unsigned int)> task(std::move(function));
			std::future<void> res(task.get_future());
			Task tsk = std::make_pair(std::move(task), std::make_pair(start, end));
			mTaskQueue.Push(std::move(tsk));
			return res;
		}

	private:
		void WaitForTask();

	private:
		bool mIsRunning;
		ThreadSafeQueue<Task> mTaskQueue;
		std::vector<std::thread> mThreads;
	};
}