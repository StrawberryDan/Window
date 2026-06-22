#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "GLFW.hpp"
// Strawberry Graphics
#include "Strawberry/Window/Event.hpp"
// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"
#include "Strawberry/Core/Sync/Spinlock.hpp"
#include "Strawberry/Core/Types/Optional.hpp"
// GLFW 3
#include "GLFW/glfw3.h"
#include "Strawberry/Window/Monitor.hpp"
// Standard Library
#include <deque>
#include <map>
#include <string>
#include <filesystem>


//======================================================================================================================
//  Foreward Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Vulkan
{
	class Surface;
}


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Window
{
	class Window
		: public GLFWUser
	{
		friend class Vulkan::Surface;
		friend void PollInput();


		static Core::Spinlock                 sInstanceMapSpinLock;
		static std::map<GLFWwindow*, Window*> sInstanceMap;


	public:
		//======================================================================================================================
		//  Construction, Destruction and Assignment
		//----------------------------------------------------------------------------------------------------------------------
		Window(const std::string& title, Core::Math::Vec2i size);
		Window(const Window& rhs)            = delete;
		Window& operator=(const Window& rhs) = delete;
		Window(Window&& rhs) noexcept;
		Window& operator=(Window&& rhs) noexcept;
		~Window();


		Monitor GetMonitor() const noexcept;


		Core::Optional<Event> NextEvent();


		bool CloseRequested() const;


		[[nodiscard]] Core::Math::Vec2i GetSize() const;
		[[nodiscard]] Core::Math::Vec2i GetFramebufferSize() const;
		[[nodiscard]] Core::Math::Vec2f GetContentScale() const;;


		bool HasFocus() const noexcept;


		const std::string& GetTitle() const;
		void               SetTitle(const std::string& title);


		void SetIcon(const std::filesystem::path& iconFile);


		void SetCursorEnabled(bool enabled);
		void SetRawMouseInputEnabled(bool enabled);

	private:
		static void OnKeyEvent(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);
		static void OnTextEvent(GLFWwindow* windowHandle, unsigned int codepoint);
		static void OnMouseMove(GLFWwindow* windowHandle, double x, double y);
		static void OnMouseButton(GLFWwindow* windowHandle, int button, int action, int mods);
		static void OnMouseScroll(GLFWwindow* windowHandle, double xOffset, double yOffset);
		static void OnWindowFocusChange(GLFWwindow* windowHandle, int focus);


		Input::Modifiers GetCurrentModifierFlags() const;


	private:
		GLFWwindow*       mHandle = nullptr;
		bool              mHasFocus = true;
		std::deque<Event> mEventQueue;

		Core::Optional<Core::Math::Vec2f> mPreviousMousePosition;

		std::string mTitle;
	};


	void PollInput();
}
