#pragma once
#include "UI.Converters.StringLocalizationConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
	namespace implementation
	{
		struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter, non_agile>, Mvvm::ObservableBase<StringLocalizationConverter>
		{
			IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, IInspectable const& parameter, hstring const& language) const;
			IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, IInspectable const& parameter, hstring const& language) const;
			DeclareObservableProperty(Windows::ApplicationModel::Resources::ResourceLoader, Context, Windows::ApplicationModel::Resources::ResourceLoader::GetForViewIndependentUse());
		};
	}

	namespace factory_implementation
	{
		struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter, implementation::StringLocalizationConverter>
		{
		};
	}
}