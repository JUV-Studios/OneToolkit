#include "UI.Windowing.WindowTitleBarController.g.h"

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;

namespace winrt::OneToolkit::UI::Windowing
{
	namespace implementation
	{
		struct WindowTitleBarController : WindowTitleBarControllerT<WindowTitleBarController>
		{
			WindowTitleBarController(ApplicationViewTitleBar const& appViewTitleBar, CoreApplicationViewTitleBar const& coreAppViewTitleBar) : AppViewTitleBar(appViewTitleBar), CoreAppViewTitleBar(coreAppViewTitleBar) {}

			auto_property<ApplicationViewTitleBar> const AppViewTitleBar;

			auto_property<CoreApplicationViewTitleBar> const CoreAppViewTitleBar;
		};
	}
	
	namespace factory_implementation
	{
		struct WindowTitleBarController : WindowTitleBarControllerT<WindowTitleBarController, implementation::WindowTitleBarController>
		{
		};
	}
}

#include "UI.Windowing.WindowTitleBarController.g.cpp"