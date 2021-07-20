#include "pch.h"
#include "WindowHelper.h"
#include "WindowHelper.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::OneToolkit::UI::WinUI::implementation
{
	int64 WindowHelper::GetWindowHandle(Window const& window)
	{
		HWND result;
		check_hresult(window.as<IWindowNative>()->get_WindowHandle(&result));
		return as_value<int64>(result);
	}
}