#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Strawberry Graphics
#include "Strawberry/Window/Input/Key.hpp"
#include "Strawberry/Window/Input/Mouse.hpp"
// Strawberry Core
#include "Strawberry/Core/Types/Variant.hpp"
#include "Strawberry/Core/Math/Vector.hpp"


namespace Strawberry::Window
{
	namespace Events
	{
		struct Key
		{
			Input::KeyCode   keyCode;
			Input::ScanCode  scanCode;
			Input::Modifiers modifiers;
			Input::KeyAction action;
		};


		struct Text
		{
			char32_t codepoint;
		};


		struct MouseMove
		{
			Core::Math::Vec2f position;
			Core::Math::Vec2f deltaPosition;
		};


		struct MouseButton
		{
			Input::MouseButton button;
			Input::Modifiers   modifiers;
			Input::KeyAction   action;
			Core::Math::Vec2f  position;
		};


		struct MouseScroll
		{
			Core::Math::Vec2f scroll;
		};


		struct Focus
		{
			bool focussed;
		};
	}


	using Event = Core::Variant<
		Events::Key,
		Events::Text,
		Events::MouseMove,
		Events::MouseButton,
		Events::MouseScroll,
		Events::Focus>;
}
