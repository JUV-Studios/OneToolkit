#include "pch.h"
#include "WindowHelper.h"
#include "WindowHelper.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::OneToolkit::UI::WinUI::implementation
{
	WindowId WindowHelper::GetWindowId(Window const& window)
	{
		if (auto nativeWindow = window.try_as<IWindowNative>())
		{
			HWND result;
			check_hresult(nativeWindow->get_WindowHandle(&result));
			return { as_value<uint64>(result) };
		}
		else
		{
			return ViewService::GetCoreWindowId(window.CoreWindow());
		}
	}
}