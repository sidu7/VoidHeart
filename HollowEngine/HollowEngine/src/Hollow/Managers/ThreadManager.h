#pragma once
#include "Hollow/Common.h"
#define MAX_THREADS 2

namespace Hollow
{
#define THREAD_FUNCTION(x,y,z) std::bind(&x,this,y,z)
	
	/// <summary>
	/// Class MultiThread Manager.
	/// </summary>
	class ThreadManager
	{
		SINGLETON(ThreadManager);
	public:
		
		/// <summary>
		/// Destructor of manager class.
		/// </summary>
		HOLLOW_API  ~ThreadManager()
		{
			CleanUp();
		}

		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API  void Init();
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API  void CleanUp();
		
		/// <summary>
		/// Pushes the specified function on task queue.
		/// </summary>
		/// <param name="function">The function.</param>
		/// <returns>std.future.</returns>
		template<typename FunctionType>
		HOLLOW_API  std::future<void> Push(const FunctionType& function)
		{
			std::packaged_task<void(unsigned int, unsigned int)> task(std::move(function));
			std::future<void> res(task.get_future());
			mTaskQueue.Push(std::move(task));
			return res;
		}

		/// <summary>
		/// Gets the thread count.
		/// </summary>
		/// <returns>size_t.</returns>
		HOLLOW_API  size_t GetThreadCount() const { return mThreads.size(); }

	private:
		void WaitForTask();

	private:
		bool mIsRunning;
		ThreadSafeQueue<std::packaged_task<void(unsigned int, unsigned int)>> mTaskQueue;
		std::vector<std::thread> mThreads;
	};
}