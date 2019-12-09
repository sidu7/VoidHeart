#pragma once

namespace Hollow
{
	/// <summary>
	/// Class ThreadSafeQueue.
	/// </summary>
	template<typename T>
	class ThreadSafeQueue 
	{
	private:    
		mutable std::mutex mMutex;    
		std::queue<T> mDataQueue;    
		std::condition_variable mDataCondition; 
	public:    
		/// <summary>
		/// Initializes a new instance of the <see cref="ThreadSafeQueue"/> class.
		/// </summary>
		HOLLOW_API ThreadSafeQueue() {}
		/// <summary>
		/// Pushes the specified new value.
		/// </summary>
		/// <param name="new_value">The new value.</param>
		HOLLOW_API void Push(T new_value)
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			mDataQueue.push(std::move(new_value));        
			mDataCondition.notify_one(); 
		}   

		/// <summary>
		/// Tries the pop.
		/// </summary>
		/// <param name="value">The value.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool TryPop(T& value)
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			if (mDataQueue.empty())            
				return false;        
			value = std::move(mDataQueue.front());        
			mDataQueue.pop();        
			return true; 
		}    
		
		/// <summary>
		/// Tries the pop.
		/// </summary>
		/// <returns>std.shared_ptr&lt;_Ty&gt;.</returns>
		HOLLOW_API std::shared_ptr<T> TryPop()
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			if (mDataQueue.empty())            
				return std::shared_ptr<T>();           
			std::shared_ptr<T> res(std::make_shared<T>(std::move(mDataQueue.front())));        
			mDataQueue.pop();        
			return res; 
		}    
		
		/// <summary>
		/// Empties this instance.
		/// </summary>
		/// <returns>bool.</returns>
		HOLLOW_API bool Empty() const
		{ 
			std::lock_guard<std::mutex> lk(mMutex);
			return mDataQueue.empty(); 
		}
	};
}
