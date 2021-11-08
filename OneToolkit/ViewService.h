#pragma once
#include "UI.ViewService.g.h"

namespace winrt::OneToolkit::UI::factory_implementation
{
	struct ViewService : ViewServiceT<ViewService, ViewService>
	{
		static Windows::UI::ViewManagement::UserInteractionMode InteractionMode();
		static OneToolkit::UI::ViewService GetForCurrentView();
		static OneToolkit::UI::ViewService GetFromWindowHandle(WindowHandle windowHandle);
		static WindowHandle GetHandleFromCoreWindow(Windows::UI::Core::CoreWindow const& coreWindow);
	};
}