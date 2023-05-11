#include "UI.Windowing.WindowController.g.h"

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::UI::Core;
using namespace Windows::UI::Core::Preview;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;

namespace winrt::OneToolkit::UI::Windowing
{
	namespace implementation
	{
		struct WindowController : WindowControllerT<WindowController>
		{
			WindowController(ApplicationView const& appView, CoreApplicationView const& coreAppView, SystemNavigationManager const& navigationManager, SystemNavigationManagerPreview const& navigationManagerPreview) : AppView(appView), CoreAppView(coreAppView),
				NavigationManager(navigationManager), NavigationManagerPreview(navigationManagerPreview) {}

			auto_property<ApplicationView> const AppView;

			auto_property<CoreApplicationView> const CoreAppView;

			auto_property<SystemNavigationManager> const NavigationManager;

			auto_property<SystemNavigationManagerPreview> const NavigationManagerPreview;
		};
	}

	namespace factory_implementation
	{
		struct WindowController : WindowControllerT<WindowController, implementation::WindowController>
		{
		};
	}
}

#include "UI.Windowing.WindowController.g.cpp"