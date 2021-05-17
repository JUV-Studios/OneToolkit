#pragma once
#include "UI.Converters.BoolReverseConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
    namespace implementation
    {
        struct BoolReverseConverter : BoolReverseConverterT<BoolReverseConverter>
        {
            BoolReverseConverter() = default;
            IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&) const noexcept;
            IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&) const noexcept;
        };
    }

    namespace factory_implementation
    {
        struct BoolReverseConverter : BoolReverseConverterT<BoolReverseConverter, implementation::BoolReverseConverter>
        {
        };
    }
}