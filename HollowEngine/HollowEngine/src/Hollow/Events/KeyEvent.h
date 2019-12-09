#pragma once

#include "Event.h"

namespace Hollow {

	/// <summary>
	/// Class KeyEvent.
	/// Implements the <see cref="Event" />
	/// </summary>
	/// <seealso cref="Event" />
	class HOLLOW_API KeyEvent : public Event
	{
	public:
		/// <summary>
		/// Gets the key code.
		/// </summary>
		/// <returns>int.</returns>
		inline int GetKeyCode() const { return mKeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: mKeyCode(keycode) {}

		int mKeyCode;
	};

	/// <summary>
	/// Class KeyPressedEvent.
	/// Implements the <see cref="KeyEvent" />
	/// </summary>
	/// <seealso cref="KeyEvent" />
	class HOLLOW_API KeyPressedEvent : public KeyEvent
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="KeyPressedEvent"/> class.
		/// </summary>
		/// <param name="keycode">The keycode.</param>
		/// <param name="repeatCount">The repeat count.</param>
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), mRepeatCount(repeatCount) {}

		/// <summary>
		/// Gets the repeat count.
		/// </summary>
		/// <returns>int.</returns>
		inline int GetRepeatCount() const { return mRepeatCount; }

		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int mRepeatCount;
	};

	/// <summary>
	/// Class KeyReleasedEvent.
	/// Implements the <see cref="KeyEvent" />
	/// </summary>
	/// <seealso cref="KeyEvent" />
	class HOLLOW_API KeyReleasedEvent : public KeyEvent
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="KeyReleasedEvent"/> class.
		/// </summary>
		/// <param name="keycode">The keycode.</param>
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	/// <summary>
	/// Class KeyTypedEvent.
	/// Implements the <see cref="KeyEvent" />
	/// </summary>
	/// <seealso cref="KeyEvent" />
	class HOLLOW_API KeyTypedEvent : public KeyEvent
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="KeyTypedEvent"/> class.
		/// </summary>
		/// <param name="keycode">The keycode.</param>
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		/// <summary>
		/// Converts to string.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}