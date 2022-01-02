#pragma once
#include "UI.ViewService.g.h"

namespace winrt::OneToolkit::UI::factory_implementation
{
	struct ViewService : ViewServiceT<ViewService, ViewService>
	{
		static Windows::UI::ViewManagement::UserInteractionMode InteractionMode();
		static OneToolkit::UI::ViewService GetForCurrentView();
		static OneToolkit::UI::ViewService GetForWindowId(Windows::UI::WindowId windowId);
		static Windows::UI::WindowId GetIdForCoreWindow(Windows::UI::Core::CoreWindow const& coreWindow);
	};
}