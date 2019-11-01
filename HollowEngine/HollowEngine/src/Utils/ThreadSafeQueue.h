#pragma once

namespace Hollow
{
	template<typename T>
	class ThreadSafeQueue 
	{
	private:    
		mutable std::mutex mMutex;    
		std::queue<T> mDataQueue;    
		std::condition_variable mDataCondition; 
	public:    
		ThreadSafeQueue() {}
		void Push(T new_value) 
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			mDataQueue.push(std::move(new_value));        
			mDataCondition.notify_one(); 
		}   
		
		void WaitAndPop(T& value) 
		{ 
			std::unique_lock<std::mutex> lk(mMutex);
			mDataCondition.wait(lk, [this] {return !mDataQueue.empty(); });        
			value = std::move(mDataQueue.front());        
			mDataQueue.pop(); 
		}   
		
		std::shared_ptr<T> WaitAndPop()
		{		
			std::unique_lock<std::mutex> lk(mMutex);
			mDataCondition.wait(lk, [this] {return !mDataQueue.empty(); });           
			std::shared_ptr<T> res(std::make_shared<T>(std::move(mDataQueue.front()))); 
			mDataQueue.pop();        
			return res;
		} 

		bool TryPop(T& value) 
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			if (mDataQueue.empty())            
				return false;        
			value = std::move(mDataQueue.front());        
			mDataQueue.pop();        
			return true; 
		}    
		
		std::shared_ptr<T> TryPop()
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			if (mDataQueue.empty())            
				return std::shared_ptr<T>();           
			std::shared_ptr<T> res(std::make_shared<T>(std::move(mDataQueue.front())));        
			mDataQueue.pop();        
			return res; 
		}    
		
		bool Empty() const 
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			return mDataQueue.empty(); 
		}
	};
}
