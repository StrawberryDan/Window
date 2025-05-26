#include "Monitor.hpp"
#include "GLFW.hpp"
// GLFW 3
#include "GLFW/glfw3.h"
// Standard :Librarry
#include <ranges>


namespace Strawberry::Window
{
	Core::Math::Vec2f MonitorInfo::GetDPI() const noexcept
	{
		Core::Math::Vec2f sizeInInches = mPhysicalSizeMM.Map([] (auto&& x) -> float { return 25.4f * x; });

		return mResolution.AsType<float>() / sizeInInches;
	}


	std::vector<MonitorInfo> GetMonitorInfo()
	{
		GLFWUser libraryUser;

		int monitorCount = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

		return std::ranges::views::counted(monitors, monitorCount)
			| std::views::transform([] (auto&& monitor)
			{
				auto videoMode = glfwGetVideoMode(monitor);
				int widthMM = 0, heightMM = 0;
				glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);

				return MonitorInfo
				{
					.mResolution = Core::Math::Vec2u(videoMode->width, videoMode->height),
					.mPhysicalSizeMM = Core::Math::Vec2u(widthMM, heightMM),
				};
			})
			| std::ranges::to<std::vector>();
	}
}
