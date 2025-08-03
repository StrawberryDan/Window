#include "GLFW.hpp"

#include "Strawberry/Core/Assert.hpp"


#include <GLFW/glfw3.h>

namespace Strawberry::Window
{
	bool GLFWLibrary::sIsInitialised = false;
	std::atomic<unsigned> GLFWUser::sInstanceCount = 0;


	bool GLFWLibrary::IsInitialised() noexcept
	{
		return sIsInitialised;
	}


	void GLFWLibrary::Initialise() noexcept
	{
		Core::Assert(!sIsInitialised);
		Core::Assert(glfwInit() == GLFW_TRUE);
		Core::Assert(glfwVulkanSupported());
		GLFWLibrary::sIsInitialised = true;
		glfwSetErrorCallback(&ErrorCallback);
	}


	void GLFWLibrary::Terminate() noexcept
	{
		Core::Assert(sIsInitialised);
		sIsInitialised = false;
		glfwTerminate();
	}


	void GLFWLibrary::ErrorCallback(int code, const char* message)
	{
		Core::Logging::Warning("GLFW_ERROR\tCode: {}\tmessage: {}", code, message);
	}


	GLFWUser::GLFWUser()
	{
		if (sInstanceCount.fetch_add(1) == 0) [[unlikely]]
		{
			GLFWLibrary::Initialise();
		}
	}


	GLFWUser::GLFWUser(const GLFWUser&) noexcept
		: GLFWUser()
	{}


	GLFWUser::GLFWUser(GLFWUser&&) noexcept
		: GLFWUser()
	{}


	GLFWUser::~GLFWUser()
	{
		if (sInstanceCount.fetch_sub(1) == 1) [[unlikely]]
		{
			GLFWLibrary::Terminate();
		}
	}
}
