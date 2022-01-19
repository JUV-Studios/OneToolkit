#include "UI.Input.KeyStateHelper.g.h"
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.Collections.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel::Core;
using namespace OneToolkit::System;

namespace winrt::OneToolkit::UI::Input
{
	namespace implementation
	{
		using GetKeyState = short(__stdcall*)(int);

		struct KeyStateHelper : static_t, KeyStateHelperT<KeyStateHelper>
		{
			static bool IsKeyUp(VirtualKey key)
			{
				return IsKeyStateActive(key, CoreVirtualKeyStates::None);
			}

			static bool IsKeyDown(VirtualKey key)
			{
				return IsKeyStateActive(key, CoreVirtualKeyStates::Down);
			}

			static bool IsKeyLocked(VirtualKey key)
			{
				return IsKeyStateActive(key, CoreVirtualKeyStates::Locked);
			}

			static bool IsKeyStateActive(VirtualKey key, CoreVirtualKeyStates keyState)
			{
				if (CoreApplication::Views().Size()) return (CoreApplication::GetCurrentView().CoreWindow().GetKeyState(key) & keyState) == keyState;
				else
				{
					DynamicLibrary user32{ L"User32.dll" };
					auto result = user32.GetProcAddress<GetKeyState>("GetKeyState")(static_cast<int>(key));
					return (static_cast<CoreVirtualKeyStates>(result) & keyState) == keyState;
				}
			}
		};
	}

	namespace factory_implementation
	{
		struct KeyStateHelper : KeyStateHelperT<KeyStateHelper, implementation::KeyStateHelper>
		{
		};
	}

}

#include "UI.Input.KeyStateHelper.g.cpp"