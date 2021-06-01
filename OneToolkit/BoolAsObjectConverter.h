#pragma once
#include "UI.Converters.BoolAsObjectConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
    namespace implementation
    {
        struct BoolAsObjectConverter : BoolAsObjectConverterT<BoolAsObjectConverter, non_agile>, Mvvm::ObservableBase<BoolAsObjectConverter>
        {
            BoolAsObjectConverter() = default;
            Mvvm::ObservableProperty<IInspectable> ValueForTrue { this, L"ValueForTrue", box_value(true) };
            Mvvm::ObservableProperty<IInspectable> ValueForFalse { this, L"ValueForFalse", box_value(false) };
            IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
            IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
        };
    }

    namespace factory_implementation
    {
        struct BoolAsObjectConverter : BoolAsObjectConverterT<BoolAsObjectConverter, implementation::BoolAsObjectConverter>
        {
        };
    }
}