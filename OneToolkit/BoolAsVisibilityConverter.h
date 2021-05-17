#pragma once
#include "UI.Converters.BoolAsVisibilityConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
    namespace implementation
    {
        struct BoolAsVisibilityConverter : BoolAsVisibilityConverterT<BoolAsVisibilityConverter>
        {
        public:
            BoolAsVisibilityConverter() = default;
            Windows::UI::Xaml::Visibility VisibilityIfTrue() const noexcept;
            void VisibilityIfTrue(Windows::UI::Xaml::Visibility value);
            Windows::UI::Xaml::Visibility VisibilityIfFalse() const noexcept;
            IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
            IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
            event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
            void PropertyChanged(event_token token) noexcept;
        private:
            Windows::UI::Xaml::Visibility m_VisibilityIfTrue;
            event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
        };
    }

    namespace factory_implementation
    {
        struct BoolAsVisibilityConverter : BoolAsVisibilityConverterT<BoolAsVisibilityConverter, implementation::BoolAsVisibilityConverter>
        {
        };
    }
}