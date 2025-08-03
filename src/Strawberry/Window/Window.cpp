//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Window.hpp"
// Strawberry Core
#include "Strawberry/Core/Assert.hpp"
// GLFW3
#include "GLFW/glfw3.h"
#include "Strawberry/Core/IO/DynamicByteBuffer.hpp"


namespace Strawberry::Window
{
	Core::Mutex<std::map<GLFWwindow*, Window*>> Window::sInstanceMap;


	Window::Window(const std::string& title, Core::Math::Vec2i size)
		: mTitle(title)
	{	ZoneScoped;
		Core::Assert(size[0] > 0 && size[1] > 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#ifdef _WIN32
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#endif
		mHandle = glfwCreateWindow(size[0], size[1], title.c_str(), nullptr, nullptr);
		glfwSetKeyCallback(mHandle, &Window::OnKeyEvent);
		glfwSetCharCallback(mHandle, &Window::OnTextEvent);
		glfwSetCursorPosCallback(mHandle, &Window::OnMouseMove);
		glfwSetMouseButtonCallback(mHandle, &Window::OnMouseButton);
		glfwSetWindowFocusCallback(mHandle, &Window::OnWindowFocusChange);
		glfwSetScrollCallback(mHandle, &Window::OnMouseScroll);

		sInstanceMap.Lock()->emplace(mHandle, this);
	}


	Window::Window(Window&& rhs) noexcept
		: mHandle(std::exchange(rhs.mHandle, nullptr))
		, mEventQueue(std::move(rhs.mEventQueue))
		, mPreviousMousePosition(std::move(rhs.mPreviousMousePosition))
	{
		sInstanceMap.Lock()->insert_or_assign(mHandle, this);
	}


	Window& Window::operator=(Window&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::destroy_at(this);
			std::construct_at(this, std::move(rhs));
		}

		return *this;
	}


	Window::~Window()
	{	ZoneScoped;
		if (mHandle)
		{
			glfwDestroyWindow(mHandle);
			sInstanceMap.Lock()->erase(mHandle);
		}
	}


	Core::Optional<Event> Window::NextEvent()
	{
		ZoneScoped;

		if (mEventQueue.empty())
		{
			return {};
		}

		auto event(mEventQueue.front());
		mEventQueue.pop_front();
		return event;
	}


	bool Window::CloseRequested() const
	{
		ZoneScoped;

		return glfwWindowShouldClose(mHandle);
	}


	Core::Math::Vec2i Window::GetSize() const
	{
		Core::Math::Vec2i size;
		glfwGetWindowSize(mHandle, &size[0], &size[1]);
		return size;
	}


	bool Window::HasFocus() const noexcept
	{
		return mHasFocus;
	}


	const std::string& Window::GetTitle() const
	{
		return mTitle;
	}


	void Window::SetTitle(const std::string& title)
	{
		mTitle = title;
		glfwSetWindowTitle(mHandle, title.c_str());
	}


	void Window::SetIcon(const std::filesystem::path& iconFile)
	{
		ZoneScoped;

		auto [size, channels, data] = Core::IO::DynamicByteBuffer::FromImage(iconFile).Unwrap();

		const GLFWimage glfwImage
		{
			.width = static_cast<int>(size[0]),
			.height = static_cast<int>(size[1]),
			.pixels = data.Data()
		};

		glfwSetWindowIcon(mHandle, 1, &glfwImage);
	}

	void Window::SetCursorEnabled(bool enabled)
	{
		ZoneScoped;

		glfwSetInputMode(mHandle, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	void Window::SetRawMouseInputEnabled(bool enabled)
	{
		ZoneScoped;

		glfwSetInputMode(mHandle, GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::OnKeyEvent(GLFWwindow* windowHandle, int key, int scancode, int action, int mods)
	{
		ZoneScoped;

		Window* window = sInstanceMap.Lock()->at(windowHandle);

		auto GetAction = [](int action)
		{
			switch (action)
			{
				case GLFW_PRESS:
					return Input::KeyAction::Press;
				case GLFW_RELEASE:
					return Input::KeyAction::Release;
				case GLFW_REPEAT:
					return Input::KeyAction::Repeat;
				default:
					Core::Unreachable();
			}
		};

		auto GetModifier = [](int modifier)
		{
			Input::Modifiers result = 0;
			if (modifier & GLFW_MOD_SHIFT) result = result | (Input::Modifiers) Input::Modifier::SHIFT;
			if (modifier & GLFW_MOD_CONTROL) result = result | (Input::Modifiers) Input::Modifier::CTRL;
			if (modifier & GLFW_MOD_ALT) result = result | (Input::Modifiers) Input::Modifier::ALT;
			if (modifier & GLFW_MOD_SUPER) result = result | (Input::Modifiers) Input::Modifier::META;
			return result;
		};


		Events::Key event
		{
			.keyCode = Input::IntoKeyCode(key).UnwrapOr(Input::KeyCode::Unknown),
			.scanCode = scancode,
			.modifiers = GetModifier(mods),
			.action = GetAction(action),
		};

		window->mEventQueue.emplace_back(event);
	}


	void Window::OnTextEvent(GLFWwindow* windowHandle, unsigned int codepoint)
	{
		ZoneScoped;

		Window* window = sInstanceMap.Lock()->at(windowHandle);

		Events::Text event {.codepoint = static_cast<char32_t>(codepoint)};

		window->mEventQueue.emplace_back(event);
	}


	void Window::OnMouseMove(GLFWwindow* windowHandle, double x, double y)
	{
		ZoneScoped;

		Window* window = sInstanceMap.Lock()->at(windowHandle);

		Core::Math::Vec2f newPos(x, y);

		Events::MouseMove event{
			.position = Core::Math::Vec2f(x, y),
			.deltaPosition = window->mPreviousMousePosition
			                       .Map([=](const auto& prev) -> Core::Math::Vec2f
			                       {
				                       return newPos - prev;
			                       })
			                       .UnwrapOr(Core::Math::Vec2f()),
		};

		window->mPreviousMousePosition = event.position;

		window->mEventQueue.emplace_back(event);
	}


	void Window::OnMouseButton(GLFWwindow* windowHandle, int button, int action, int mods)
	{
		ZoneScoped;

		Window* window = sInstanceMap.Lock()->at(windowHandle);

		auto GetButton = [](int code)
		{
			switch (code)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					return Input::MouseButton::Left;
				case GLFW_MOUSE_BUTTON_RIGHT:
					return Input::MouseButton::Right;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					return Input::MouseButton::Middle;
				default:
					Core::Unreachable();
			}
		};

		auto GetAction = [](int action)
		{
			switch (action)
			{
				case GLFW_PRESS:
					return Input::KeyAction::Press;
				case GLFW_RELEASE:
					return Input::KeyAction::Release;
				case GLFW_REPEAT:
					return Input::KeyAction::Repeat;
				default:
					Core::Unreachable();
			}
		};

		auto GetModifier = [](int modifier)
		{
			Input::Modifiers result = 0;
			if (modifier & GLFW_MOD_SHIFT) result = result | static_cast<Input::Modifiers>(Input::Modifier::SHIFT);
			if (modifier & GLFW_MOD_CONTROL) result = result | static_cast<Input::Modifiers>(Input::Modifier::CTRL);
			if (modifier & GLFW_MOD_ALT) result = result | static_cast<Input::Modifiers>(Input::Modifier::ALT);
			if (modifier & GLFW_MOD_SUPER) result = result | static_cast<Input::Modifiers>(Input::Modifier::META);
			return result;
		};


		Core::Math::Vec2 position;
		glfwGetCursorPos(windowHandle, &position[0], &position[1]);


		Events::MouseButton event{
			.button = GetButton(button),
			.modifiers = GetModifier(mods),
			.action = GetAction(action),
			.position = position.AsType<float>()
		};

		window->mEventQueue.emplace_back(event);
	}


	void Window::OnMouseScroll(GLFWwindow* windowHandle, double xOffset, double yOffset)
	{
		ZoneScoped;

		Window* window = sInstanceMap.Lock()->at(windowHandle);

		Events::MouseScroll event
		{
			.scroll = {xOffset, yOffset},
		};

		window->mEventQueue.emplace_back(event);
	}


	void Window::OnWindowFocusChange(GLFWwindow* windowHandle, int focus)
	{
		ZoneScoped;

		Window* window    = sInstanceMap.Lock()->at(windowHandle);
		window->mHasFocus = focus == GLFW_TRUE;
		window->mEventQueue.emplace_back(Events::Focus{.focussed = window->HasFocus()});
	}


	Input::Modifiers Window::GetCurrentModifierFlags() const
	{
		ZoneScoped;

		Input::Modifiers modifiers = 0;

		if (glfwGetKey(mHandle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
			|| glfwGetKey(mHandle, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		{
			modifiers = modifiers | Input::Modifier::CTRL;
		}

		if (glfwGetKey(mHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			|| glfwGetKey(mHandle, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			modifiers = modifiers | Input::Modifier::SHIFT;
		}

		if (glfwGetKey(mHandle, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
			|| glfwGetKey(mHandle, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
		{
			modifiers = modifiers | Input::Modifier::ALT;
		}

		if (glfwGetKey(mHandle, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS
			|| glfwGetKey(mHandle, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS)
		{
			modifiers = modifiers | Input::Modifier::META;
		}

		return modifiers;
	}


	void PollInput()
	{
		ZoneScoped;

		glfwPollEvents();


		// Insert a hold event for every key that is currently held down.
		for (auto&& window : *Window::sInstanceMap.Lock())
		{
			const Input::Modifiers modifiers = window.second->GetCurrentModifierFlags();
			for (int i = 0; i < GLFW_KEY_LAST; i++)
			{
				auto keycode = Input::IntoKeyCode(i);
				if (!keycode) continue;

				auto scanCode = glfwGetKeyScancode(i);
				if (scanCode == -1) continue;

				if (glfwGetKey(window.second->mHandle, i) == GLFW_PRESS)
				{
					Events::Key holdEvent
					{
						.keyCode = keycode.Value(),
						.scanCode = scanCode,
						.modifiers = modifiers,
						.action = Input::KeyAction::Hold
					};

					window.second->mEventQueue.emplace_back(holdEvent);
				}
			}
		}
	}
}
