#pragma once
#include "UI.Converters.BoolAsVisibilityConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
    namespace implementation
    {
        struct BoolAsVisibilityConverter : BoolAsVisibilityConverterT<BoolAsVisibilityConverter, non_agile>, Mvvm::ObservableBase<BoolAsVisibilityConverter>
        {
        public:
            BoolAsVisibilityConverter() = default;
            Mvvm::ObservableProperty<Windows::UI::Xaml::Visibility> VisibilityIfTrue { this, L"VisibilityIfTrue" };
            Windows::UI::Xaml::Visibility VisibilityIfFalse() const noexcept;
            IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
            IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, IInspectable const&, hstring const&);
        protected:
            void WhenPropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventArgs const& args) override final;
        };
    }

    namespace factory_implementation
    {
        struct BoolAsVisibilityConverter : BoolAsVisibilityConverterT<BoolAsVisibilityConverter, implementation::BoolAsVisibilityConverter>
        {
        };
    }
}