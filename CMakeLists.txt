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
	if (NOT TARGET StrawberryCore AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Core)
		add_subdirectory(../Core ${CMAKE_CURRENT_BINARY_DIR}/Core)
	endif ()


	FetchContent_Declare(GLFW
		EXCLUDE_FROM_ALL
		GIT_REPOSITORY "https://github.com/glfw/glfw.git"
		GIT_TAG "3.3.8")
	FetchContent_MakeAvailable(GLFW)


	add_library(StrawberryWindow STATIC
		src/Strawberry/Window/Input/Key.cpp
		src/Strawberry/Window/Input/Key.hpp
		src/Strawberry/Window/Input/Mouse.hpp
		src/Strawberry/Window/Event.hpp
		src/Strawberry/Window/Window.cpp
		src/Strawberry/Window/Window.hpp)
	target_include_directories(StrawberryWindow PUBLIC src)
	target_link_libraries(StrawberryWindow PUBLIC StrawberryCore glfw)
	set_target_properties(StrawberryWindow PROPERTIES CXX_STANDARD 23)


	add_executable(StrawberryWindowTest
		test/Main.cpp)
	target_link_libraries(StrawberryWindowTest PUBLIC StrawberryWindow)
	set_target_properties(StrawberryWindowTest PROPERTIES CXX_STANDARD 23)
endif ()