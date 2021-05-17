#pragma once
#include "Reflection.Activator.g.h"

namespace winrt::OneToolkit::Reflection
{
    namespace implementation
    {
        struct Activator : ActivatorT<Activator>
        {
            Activator() = delete;
            static IInspectable ActivateInstance(Windows::UI::Xaml::Interop::TypeName const& typeId, array_view<IInspectable const> args = {});
        };
    }

    namespace factory_implementation
    {
        struct Activator : ActivatorT<Activator, implementation::Activator>
        {
        };
    }
}