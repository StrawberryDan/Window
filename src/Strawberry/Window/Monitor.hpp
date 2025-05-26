#pragma once
// Includes
//
// GLFW 3
#include "GLFW.hpp"
// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"
// Standard Library
#include <vector>


namespace Strawberry::Window
{
	struct MonitorInfo
		: GLFWUser
	{
		Core::Math::Vec2u mResolution;
		Core::Math::Vec2u mPhysicalSizeMM;


		Core::Math::Vec2f GetDPI() const noexcept;
	};


	std::vector<MonitorInfo> GetMonitorInfo();
}
