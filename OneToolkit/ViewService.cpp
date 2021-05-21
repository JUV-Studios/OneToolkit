#include "pch.h"
#include "OneToolkit.h"
#include "ViewService.h"
#include "ViewServiceUwp.h"
#include "ViewServiceWin32.h"
#include "UI.ViewService.g.cpp"

namespace winrt::OneToolkit::UI::implementation
{
	IViewServiceProvider ViewService::GetForCurrentView()
	{
		return make<ViewServiceUwp>();
	}

	IViewServiceProvider ViewService::GetForWindowId(int64_t windowHandle)
	{
		auto hwnd = juv::as_pointer<HWND>(windowHandle);
		return make<ViewServiceWin32>(hwnd);
	}
}