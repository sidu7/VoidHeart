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
		HOLLOW_API ThreadSafeQueue() {}
		HOLLOW_API void Push(T new_value)
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			mDataQueue.push(std::move(new_value));        
			mDataCondition.notify_one(); 
		}   

		HOLLOW_API bool TryPop(T& value)
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			if (mDataQueue.empty())            
				return false;        
			value = std::move(mDataQueue.front());        
			mDataQueue.pop();        
			return true; 
		}    
		
		HOLLOW_API std::shared_ptr<T> TryPop()
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			if (mDataQueue.empty())            
				return std::shared_ptr<T>();           
			std::shared_ptr<T> res(std::make_shared<T>(std::move(mDataQueue.front())));        
			mDataQueue.pop();        
			return res; 
		}    
		
		HOLLOW_API bool Empty() const
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			return mDataQueue.empty(); 
		}
	};
}
