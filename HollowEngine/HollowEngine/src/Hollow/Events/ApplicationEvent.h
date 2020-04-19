#pragma once

#include "Event.h"

namespace Hollow {

	class HOLLOW_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: mWidth(width), mHeight(height) {}

		inline unsigned int GetWidth() const { return mWidth; }
		inline unsigned int GetHeight() const { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int mWidth, mHeight;
	};

	class HOLLOW_API WindowFullScreenEvent : public Event
	{
	public:
		WindowFullScreenEvent() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowFullScreenEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowFullScreen)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class HOLLOW_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class HOLLOW_API WindowFocusLostEvent : public Event
	{
	public:
		WindowFocusLostEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class HOLLOW_API WindowFocusGainedEvent : public Event
	{
	public:
		WindowFocusGainedEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class HOLLOW_API WindowPausedEvent : public Event
	{
	public:
		WindowPausedEvent() {}

		EVENT_CLASS_TYPE(WindowPaused)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}