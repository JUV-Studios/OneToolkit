#include "pch.h"
#include "KeyStateHelper.h"
#include "UI.Input.KeyStateHelper.g.cpp"

using namespace winrt;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel::Core;
using namespace OneToolkit::Runtime;

namespace winrt::OneToolkit::UI::Input::implementation
{
	typedef SHORT(__stdcall* GetKeyState)(int);

	bool KeyStateHelper::IsKeyUp(VirtualKey key)
	{
		return IsKeyStateActive(key, CoreVirtualKeyStates::None);
	}

	bool KeyStateHelper::IsKeyDown(VirtualKey key)
	{
		return IsKeyStateActive(key, CoreVirtualKeyStates::Down);
	}

	bool KeyStateHelper::IsKeyLocked(VirtualKey key)
	{
		return IsKeyStateActive(key, CoreVirtualKeyStates::Locked);
	}

	bool KeyStateHelper::IsKeyStateActive(VirtualKey key, CoreVirtualKeyStates keyState)
	{
		if (CoreApplication::Views().Size() == 0)
		{
			DynamicModule user32 { L"User32.dll" };
			auto result = user32.GetProcAddress<GetKeyState>("GetKeyState")(static_cast<int>(key));
			return (static_cast<CoreVirtualKeyStates>(result) & keyState) == keyState;
		}
		else
		{
			return (CoreApplication::GetCurrentView().CoreWindow().GetKeyState(key) & keyState) == keyState;
		}
	}
}