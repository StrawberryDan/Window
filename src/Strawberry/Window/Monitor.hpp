#pragma once
// Includes
//
// GLFW 3
#include "GLFW.hpp"
// Strawberry Core
#include "GLFW/glfw3.h"
#include "Strawberry/Core/Math/Vector.hpp"


namespace Strawberry::Window
{
	class Monitor
		: GLFWUser
	{
		friend class Window;


	public:
		Core::Math::Vec2u GetCurrentResolution() const noexcept;

		Core::Math::Vec2f GetContentScale() const noexcept;

		Core::Math::Vec2i GetPhysicalSizeMM() const noexcept;
		Core::Math::Vec2  GetPhysicalSizeIn() const noexcept;
		Core::Math::Vec2  GetDPI() const noexcept;


	private:
		Monitor(GLFWmonitor* monitor) noexcept;


		GLFWmonitor* mMonitor;
	};
}
