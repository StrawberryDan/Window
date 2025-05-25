cmake_minimum_required(VERSION 3.25)
project(StrawberryWindow)
include(FetchContent)


if (PROJECT_IS_TOP_LEVEL)
	FetchContent_Declare(StrawberryConfig
		GIT_REPOSITORY "https://github.com/StrawberryDan/Config.git"
		GIT_TAG "main")
	FetchContent_MakeAvailable(StrawberryConfig)
	include(StrawberryConfig)
endif ()


if (NOT TARGET StrawberryWindow)
	find_strawberry_library(NAME Core)


	FetchContent_Declare(GLFW
		EXCLUDE_FROM_ALL
		GIT_REPOSITORY "https://github.com/glfw/glfw.git"
		GIT_TAG "3.3.8")
	FetchContent_MakeAvailable(GLFW)


	list(APPEND StrawberryWindow_Sources
		src/Strawberry/Window/Event.hpp
		src/Strawberry/Window/Input/Key.cpp
		src/Strawberry/Window/Input/Key.hpp
		src/Strawberry/Window/Input/Mouse.hpp
		src/Strawberry/Window/Monitor.cpp
		src/Strawberry/Window/Monitor.hpp
		src/Strawberry/Window/Window.cpp
		src/Strawberry/Window/Window.hpp)


	new_strawberry_library(NAME StrawberryWindow SOURCE ${StrawberryWindow_Sources})
	target_include_directories(StrawberryWindow PUBLIC src)
	target_link_libraries(StrawberryWindow PUBLIC StrawberryCore glfw)
	set_target_properties(StrawberryWindow PROPERTIES CXX_STANDARD 23)


	add_executable(StrawberryWindowTest
		test/Main.cpp)
	target_link_libraries(StrawberryWindowTest PUBLIC StrawberryWindow)
	set_target_properties(StrawberryWindowTest PROPERTIES CXX_STANDARD 23)
endif ()