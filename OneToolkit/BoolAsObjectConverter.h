#pragma once
#include "UI.Converters.BoolAsObjectConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
    namespace implementation
    {
        struct BoolAsObjectConverter : BoolAsObjectConverterT<BoolAsObjectConverter>
        {
        public:
            BoolAsObjectConverter() = default;
            IInspectable ValueForTrue() const noexcept;
            void ValueForTrue(IInspectable const& value);
            IInspectable ValueForFalse() const noexcept;
            void ValueForFalse(IInspectable const& value);
            IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
            IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
            event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
            void PropertyChanged(event_token token) noexcept;
        private:
            IInspectable m_TrueValue = box_value(true);
            IInspectable m_FalseValue = box_value(false);
            event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
        };
    }

    namespace factory_implementation
    {
        struct BoolAsObjectConverter : BoolAsObjectConverterT<BoolAsObjectConverter, implementation::BoolAsObjectConverter>
        {
        };
    }
}