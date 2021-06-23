#pragma once
#include "UI.Input.KeyStateHelper.g.h"

namespace winrt::OneToolkit::UI::Input
{
	namespace implementation
	{
		struct KeyStateHelper : KeyStateHelperT<KeyStateHelper>
		{
			KeyStateHelper() = delete;
			static bool IsKeyUp(Windows::System::VirtualKey key);
			static bool IsKeyDown(Windows::System::VirtualKey key);
			static bool IsKeyLocked(Windows::System::VirtualKey key);
			static bool IsKeyStateActive(Windows::System::VirtualKey key, Windows::UI::Core::CoreVirtualKeyStates keyState);
		};
	}

	namespace factory_implementation
	{
		struct KeyStateHelper : KeyStateHelperT<KeyStateHelper, implementation::KeyStateHelper>
		{
		};
	}
}