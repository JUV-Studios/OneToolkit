#pragma once
#include "WindowHelper.g.h"

namespace winrt::OneToolkit::UI::WinUI
{
    namespace implementation
    {
        struct WindowHelper : WindowHelperT<WindowHelper>
        {
            WindowHelper() = delete;
            static WindowId GetWindowId(Microsoft::UI::Xaml::Window const& window);
        };
    }

    namespace factory_implementation
    {
        struct WindowHelper : WindowHelperT<WindowHelper, implementation::WindowHelper>
        {
        };
    }
}