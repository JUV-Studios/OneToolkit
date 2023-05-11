#pragma once

#include "Class.g.h"

namespace winrt::OneToolkit_UI_Xaml_Controls_WinUI::implementation
{
    struct Class : ClassT<Class>
    {
        Class() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::OneToolkit_UI_Xaml_Controls_WinUI::factory_implementation
{
    struct Class : ClassT<Class, implementation::Class>
    {
    };
}
