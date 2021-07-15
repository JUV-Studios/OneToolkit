#pragma once
#include "UI.ViewService.g.h"

namespace winrt::OneToolkit::UI
{
    namespace implementation
    {
        struct ViewService : ViewServiceT<ViewService>
        {
            ViewService() = delete;
            static Windows::UI::ViewManagement::UserInteractionMode InteractionMode();
            static IViewServiceUniversal GetForCurrentView();
            static IViewServiceProvider GetForWindowId(int64_t windowHandle);
        };
    }
    
    namespace factory_implementation
    {
        struct ViewService : ViewServiceT<ViewService, implementation::ViewService>
        {
        };
    }
}