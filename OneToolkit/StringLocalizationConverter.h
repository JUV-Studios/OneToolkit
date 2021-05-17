#pragma once
#include "UI.Converters.StringLocalizationConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
    namespace implementation
    {
        struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter>
        {
        public:
            StringLocalizationConverter() = default;
            Windows::ApplicationModel::Resources::ResourceLoader Context() const noexcept;
            void Context(Windows::ApplicationModel::Resources::ResourceLoader const& value);
            event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
            void PropertyChanged(event_token const& token) noexcept;
            IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, IInspectable const& parameter, hstring const& language);
            IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, IInspectable const& parameter, hstring const& language);
        private:
            event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
            Windows::ApplicationModel::Resources::ResourceLoader m_Context = Windows::ApplicationModel::Resources::ResourceLoader::GetForViewIndependentUse();
        };
    }

    namespace factory_implementation
    {
        struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter, implementation::StringLocalizationConverter>
        {
        };
    }
}