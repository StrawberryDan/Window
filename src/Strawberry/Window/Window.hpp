#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Strawberry Graphics
#include "Strawberry/Window/Event.hpp"
// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"
#include "Strawberry/Core/Types/Optional.hpp"
#include "Strawberry/Core/Sync/Mutex.hpp"
// GLFW 3
#include "GLFW/glfw3.h"
// Standard Library
#include <atomic>
#include <concepts>
#include <deque>
#include <map>
#include <string>
#include <filesystem>


//======================================================================================================================
//  Foreward Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Graphics::Vulkan { class Surface; }


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Window
{
	class Window
	{
		friend class Graphics::Vulkan::Surface;


	private:
		static void Initialise();
		static void Terminate();


		static std::atomic<unsigned int> sInstanceCount;
		static Core::Mutex<std::map<GLFWwindow*, Window*>> sInstanceMap;


	public:
		//======================================================================================================================
		//  Construction, Destruction and Assignment
		//----------------------------------------------------------------------------------------------------------------------
		Window(const std::string& title, Core::Math::Vec2i size);
		Window(const Window& rhs) = default;
		Window& operator=(const Window& rhs) = default;
		Window(Window&& rhs) noexcept;
		Window& operator=(Window&& rhs) noexcept;
		~Window();

		Core::Optional<Event> NextEvent();

		bool CloseRequested() const;

		void SwapBuffers();

		[[nodiscard]] Core::Math::Vec2i GetSize() const;

		template <std::movable T, typename... Args> requires (std::constructible_from<T, const Window&, Args...>)
		T Create(const Args&... args) { return T(*this, std::forward<const Args&>(args)...); }


		const std::string& GetTitle() const;
		void SetTitle(const std::string& title);


		void SetIcon(const std::filesystem::path& iconFile);


	private:
		static void OnKeyEvent(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);
		static void OnTextEvent(GLFWwindow* windowHandle, unsigned int codepoint);
		static void OnMouseMove(GLFWwindow* windowHandle, double x, double y);
		static void OnMouseButton(GLFWwindow* windowHandle, int button, int action, int mods);


		Core::Math::Vec2f ScaleCoordinate(Core::Math::Vec2 in) const;


	private:
		GLFWwindow* mHandle;
		std::deque<Event> mEventQueue;


		Core::Math::Vec2u mLogicalSize;

		Core::Optional<Core::Math::Vec2f> mPreviousMousePosition;


		std::string mTitle;
	};


	void PollInput();
}
