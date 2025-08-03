#pragma once


#include <atomic>


namespace Strawberry::Window
{
	class GLFWLibrary
	{
		friend class GLFWUser;

	public:
		static bool IsInitialised() noexcept;

	private:
		static void Initialise() noexcept;
		static void Terminate() noexcept;
		static bool sIsInitialised;

		static void ErrorCallback(int, const char*);
	};


	class GLFWUser
	{
	public:
		GLFWUser();
		GLFWUser(const GLFWUser&) noexcept;
		GLFWUser(GLFWUser&&) noexcept;
		~GLFWUser();

	private:
		static std::atomic<unsigned int> sInstanceCount;
	};
}