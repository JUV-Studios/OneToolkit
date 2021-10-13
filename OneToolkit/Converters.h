#pragma once
#include "UI.Converters.NullToBoolConverter.g.h"
#include "UI.Converters.VisibilityToOpacityConverter.g.h"
#include "UI.Converters.BoolReverseConverter.g.h"
#include "UI.Converters.StringTypeIdConverter.g.h"
#include "UI.Converters.StringVersionConverter.g.h"
#include "UI.Converters.StringFileSizeConverter.g.h"
#include "UI.Converters.StringLocalizationConverter.g.h"
#include "UI.Converters.StringToVisibilityConverter.g.h"

namespace winrt::OneToolkit::UI::Converters
{
	namespace implementation
	{
		struct NullToBoolConverter : NullToBoolConverterT<NullToBoolConverter>
		{
			IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language) const;
			IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language) const;
		};

		struct VisibilityToOpacityConverter : VisibilityToOpacityConverterT<VisibilityToOpacityConverter>
		{
			IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language) const;
			IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language) const;
		};

		struct BoolReverseConverter : BoolReverseConverterT<BoolReverseConverter>, TwoWayConverter<BoolReverseConverter>
		{
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringTypeIdConverter : StringTypeIdConverterT<StringTypeIdConverter>, TwoWayConverter<StringTypeIdConverter>
		{
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringVersionConverter : StringVersionConverterT<StringVersionConverter>, TwoWayConverter<StringVersionConverter>
		{
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringFileSizeConverter : StringFileSizeConverterT<StringFileSizeConverter>, TwoWayConverter<StringFileSizeConverter>, Mvvm::ObservableBase<StringFileSizeConverter>
		{
			DeclareObservableProperty(bool, UseBinaryPrefix, false);
			IInspectable ConvertValue(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};

		struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter>, Mvvm::ObservableBase<StringLocalizationConverter>
		{
			DeclareObservableProperty(Windows::ApplicationModel::Resources::ResourceLoader, Context, Windows::ApplicationModel::Resources::ResourceLoader::GetForViewIndependentUse());
			IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language) const;
			IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language) const;
		};

		struct StringToVisibilityConverter : StringToVisibilityConverterT<StringToVisibilityConverter>
		{
			IInspectable Convert(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
			IInspectable ConvertBack(IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& parameter, hstring const& language);
		};
	}

	namespace factory_implementation
	{
		struct NullToBoolConverter : NullToBoolConverterT<NullToBoolConverter, implementation::NullToBoolConverter>
		{
		};

		struct VisibilityToOpacityConverter : VisibilityToOpacityConverterT<VisibilityToOpacityConverter, implementation::VisibilityToOpacityConverter>
		{
		};

		struct BoolReverseConverter : BoolReverseConverterT<BoolReverseConverter, implementation::BoolReverseConverter>
		{
		};

		struct StringTypeIdConverter : StringTypeIdConverterT<StringTypeIdConverter, implementation::StringTypeIdConverter>
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

		struct StringToVisibilityConverter : StringToVisibilityConverterT<StringToVisibilityConverter, implementation::StringToVisibilityConverter>
		{
		};
	}
}