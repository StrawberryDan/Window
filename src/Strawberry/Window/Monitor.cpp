#include "Monitor.hpp"
// GLFW 3
#include "GLFW/glfw3.h"
// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"
// Standard :Librarry


namespace Strawberry::Window
{
	Core::Math::Vec2u Monitor::GetCurrentResolution() const noexcept
	{
		const GLFWvidmode* videomode = glfwGetVideoMode(mMonitor);
		return { videomode->width, videomode->height };
	}


	Core::Math::Vec2f Monitor::GetContentScale() const noexcept
	{
		float x = 0.0, y = 0.0;
		glfwGetMonitorContentScale(mMonitor, &x, &y);
		return {x, y};
	}


	Core::Math::Vec2i Monitor::GetPhysicalSizeMM() const noexcept
	{
		int width = 0, height = 0;
		glfwGetMonitorPhysicalSize(mMonitor, &width, &height);
		return { width, height };
	}

	Core::Math::Vec2 Monitor::GetPhysicalSizeIn() const noexcept
	{
		static constexpr double MM_TO_INCHES = 0.0393701;
		return GetPhysicalSizeMM().Map([&] (auto&& x) { return x * MM_TO_INCHES; });
	}


	Core::Math::Vec2 Monitor::GetDPI() const noexcept
	{
		auto sizeIN = GetPhysicalSizeIn();
		auto resolution = GetCurrentResolution();
		return sizeIN.Piecewise(std::divides{}, resolution.AsType<double>());
	}


	Monitor::Monitor(GLFWmonitor* monitor) noexcept
		: mMonitor(monitor)
	{}
}
