#include "Strawberry/Window/Window.hpp"


int main()
{
	using namespace Strawberry::Window;

	Strawberry::Window::Window window("Strawberry Window Test", {1920, 1080});

	while (!window.CloseRequested())
	{
		Strawberry::Window::PollInput();
	}

	return 0;
}
