#pragma once
#include "Mvvm.NonGeneric.ObservableValue.g.h"

namespace winrt::OneToolkit::Mvvm::NonGeneric
{
    namespace implementation
    {
        struct ObservableValue : ObservableValueT<ObservableValue>, Mvvm::ObservableBase<ObservableValue>
        {
            ObservableValue(IInspectable const& val = {}, bool suppressEvents = false);
            Mvvm::ObservableProperty<IInspectable> Value { this, L"Value" };
            bool IsEqual(IInspectable const& another) const noexcept;
            IInspectable Clone();
        };
    }

    namespace factory_implementation
    {
        struct ObservableValue : ObservableValueT<ObservableValue, implementation::ObservableValue>
        {
        };
    }
}