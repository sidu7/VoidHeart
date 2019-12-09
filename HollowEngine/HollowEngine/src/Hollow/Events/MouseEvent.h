#pragma once

#include "Event.h"

namespace Hollow {

	/// <summary>
	/// Class MouseMovedEvent.
	/// Implements the <see cref="Event" />
	/// </summary>
	/// <seealso cref="Event" />
	class HOLLOW_API MouseMovedEvent : public Event
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MouseMovedEvent"/> class.
		/// </summary>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		MouseMovedEvent(float x, float y)
			: mMouseX(x), mMouseY(y) {}

		/// <summary>
		/// Gets the x position.
		/// </summary>
		/// <returns>float.</returns>
		inline float GetX() const { return mMouseX; }
		/// <summary>
		/// Gets the y position.
		/// </summary>
		/// <returns>float.</returns>
		inline float GetY() const { return mMouseY; }

		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mMouseX, mMouseY;
	};

	/// <summary>
	/// Class MouseScrolledEvent.
	/// Implements the <see cref="Event" />
	/// </summary>
	/// <seealso cref="Event" />
	class HOLLOW_API MouseScrolledEvent : public Event
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MouseScrolledEvent"/> class.
		/// </summary>
		/// <param name="xOffset">The x offset.</param>
		/// <param name="yOffset">The y offset.</param>
		MouseScrolledEvent(float xOffset, float yOffset)
			: mXOffset(xOffset), mYOffset(yOffset) {}

		/// <summary>
		/// Gets the x offset.
		/// </summary>
		/// <returns>float.</returns>
		inline float GetXOffset() const { return mXOffset; }
		/// <summary>
		/// Gets the y offset.
		/// </summary>
		/// <returns>float.</returns>
		inline float GetYOffset() const { return mYOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mXOffset, mYOffset;
	};

	/// <summary>
	/// Class MouseButtonEvent.
	/// Implements the <see cref="Event" />
	/// </summary>
	/// <seealso cref="Event" />
	class HOLLOW_API MouseButtonEvent : public Event
	{
	public:
		/// <summary>
		/// Gets the mouse button.
		/// </summary>
		/// <returns>int.</returns>
		inline int GetMouseButton() const { return mButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: mButton(button) {}

		int mButton;
	};

	/// <summary>
	/// Class MouseButtonPressedEvent.
	/// Implements the <see cref="MouseButtonEvent" />
	/// </summary>
	/// <seealso cref="Event" />
	class HOLLOW_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MouseButtonPressedEvent"/> class.
		/// </summary>
		/// <param name="button">The button.</param>
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class HOLLOW_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MouseButtonReleasedEvent"/> class.
		/// </summary>
		/// <param name="button">The button.</param>
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}