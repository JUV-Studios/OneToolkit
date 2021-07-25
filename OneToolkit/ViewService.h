#pragma once
#include "UI.ViewService.g.h"

namespace winrt::OneToolkit::UI
{
    namespace factory_implementation
    {
        struct ViewService : ViewServiceT<ViewService, ViewService>
        {
            static Windows::UI::ViewManagement::UserInteractionMode InteractionMode();
            static OneToolkit::UI::ViewService GetForCurrentView();
            static OneToolkit::UI::ViewService GetForWindowId(juv::int64 windowHandle);
            static juv::int64 GetWindowHandle(Windows::UI::Core::CoreWindow const& coreWindow);
        };
    }
}