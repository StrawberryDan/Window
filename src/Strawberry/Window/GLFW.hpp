#pragma once


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