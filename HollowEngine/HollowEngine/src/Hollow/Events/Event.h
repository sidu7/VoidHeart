#pragma once

#include <hollowpch.h>

#include "Hollow/Core/Core.h"

namespace Hollow {

	// Events in Hollow are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	/// <summary>
	/// Enum EventType
	/// </summary>
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, WindowFullScreen,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	/// <summary>
	/// Enum EventCategory
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	/// <summary>
	/// Class Event.
	/// </summary>
	class HOLLOW_API Event
	{
	public:
		bool Handled = false;

		/// <summary>
		/// Gets the type of the event.
		/// </summary>
		/// <returns>Hollow.EventType.</returns>
		virtual EventType GetEventType() const = 0;
		/// <summary>
		/// Gets the name.
		/// </summary>
		/// <returns>const char *.</returns>
		virtual const char* GetName() const = 0;
		/// <summary>
		/// Gets the category flags.
		/// </summary>
		/// <returns>int.</returns>
		virtual int GetCategoryFlags() const = 0;
		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		virtual std::string ToString() const { return GetName(); }

		/// <summary>
		/// Destructor for the <see cref="Event"/> class.
		/// </summary>
		virtual ~Event() {}
		
		/// <summary>
		/// Determines whether [is in category] [the specified category].
		/// </summary>
		/// <param name="category">The category.</param>
		/// <returns>bool.</returns>
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	/// <summary>
	/// Class EventDispatcher.
	/// </summary>
	class EventDispatcher
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="EventDispatcher"/> class.
		/// </summary>
		/// <param name="event">The event.</param>
		EventDispatcher(Event& event)
			: mEvent(event)
		{
		}
		
		// F will be deduced by the compiler
		/// <summary>
		/// Dispatches the specified function if it is the right type.
		/// </summary>
		/// <param name="func">The function.</param>
		/// <returns>If event is handled returns true or false.</returns>
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (mEvent.GetEventType() == T::GetStaticType())
			{
				mEvent.Handled = func(static_cast<T&>(mEvent));
				return true;
			}
			return false;
		}
	private:
		Event& mEvent;
	};

	/// <summary>
	/// Overloads the extraction operator. 
	/// </summary>
	/// <param name="os">The output stream.</param>
	/// <param name="e">The event.</param>
	/// <returns>std.ostream &.</returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

