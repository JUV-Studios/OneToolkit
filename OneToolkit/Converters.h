#pragma once
#include "UI.Converters.StringTypeConverter.g.h"
#include "UI.Converters.StringVersionConverter.g.h"
#include "UI.Converters.StringFileSizeConverter.g.h"
#include "UI.Converters.StringLocalizationConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
	template <typename Derived>
	struct TwoWayConverterBase
	{
		Windows::Foundation::IInspectable Convert(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language)
		{
			return static_cast<Derived*>(this)->ConvertValue(value, targetType, parameter, language);
		}

		Windows::Foundation::IInspectable ConvertBack(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language)
		{
			return static_cast<Derived*>(this)->ConvertValue(value, targetType, parameter, language);
		}
	};

	namespace implementation
	{
		struct StringTypeConverter : StringTypeConverterT<StringTypeConverter>, TwoWayConverterBase<StringTypeConverter>
		{
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringVersionConverter : StringVersionConverterT<StringVersionConverter>, TwoWayConverterBase<StringVersionConverter>
		{
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringFileSizeConverter : StringFileSizeConverterT<StringFileSizeConverter>, TwoWayConverterBase<StringFileSizeConverter>, Mvvm::ObservableBase<StringFileSizeConverter>
		{
			DeclareObservableProperty(bool, UseBinaryPrefix, false);
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter>, TwoWayConverterBase<StringLocalizationConverter>, Mvvm::ObservableBase<StringLocalizationConverter>
		{
			DeclareObservableProperty(Windows::ApplicationModel::Resources::ResourceLoader, Context, Windows::ApplicationModel::Resources::ResourceLoader::GetForViewIndependentUse());
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};
	}

	namespace factory_implementation
	{
		struct StringTypeConverter : StringTypeConverterT<StringTypeConverter, implementation::StringTypeConverter>
		{
		};

		struct StringVersionConverter : StringVersionConverterT<StringVersionConverter, implementation::StringVersionConverter>
		{
		};

		struct StringFileSizeConverter : StringFileSizeConverterT<StringFileSizeConverter, implementation::StringFileSizeConverter>
		{
		};

		struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter, implementation::StringLocalizationConverter>
		{
		};
	}
}