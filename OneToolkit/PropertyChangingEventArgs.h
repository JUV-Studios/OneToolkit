#pragma once
#include "Mvvm.PropertyChangingEventArgs.g.h"

namespace winrt::OneToolkit::Mvvm
{
    namespace implementation
    {
        struct PropertyChangingEventArgs : PropertyChangingEventArgsT<PropertyChangingEventArgs>
        {
            PropertyChangingEventArgs(hstring const& propertyName);
            hstring PropertyName() const noexcept;
        private:
            hstring m_PropertyName;
        };
    }

    namespace factory_implementation
    {
        struct PropertyChangingEventArgs : PropertyChangingEventArgsT<PropertyChangingEventArgs, implementation::PropertyChangingEventArgs>
        {
        };
    }
}