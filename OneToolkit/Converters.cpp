#include "pch.h"
#include "Converters.h"
#include "UI.Converters.NullToBoolConverter.g.cpp"
#include "UI.Converters.VisibilityToOpacityConverter.g.cpp"
#include "UI.Converters.BoolReverseConverter.g.cpp"
#include "UI.Converters.StringTypeIdConverter.g.cpp"
#include "UI.Converters.StringVersionConverter.g.cpp"
#include "UI.Converters.StringFileSizeConverter.g.cpp"
#include "UI.Converters.StringLocalizationConverter.g.cpp"
#include "UI.Converters.StringToVisibilityConverter.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace OneToolkit::System;
using namespace OneToolkit::Storage;
using namespace OneToolkit::Runtime;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	IInspectable NullToBoolConverter::Convert(IInspectable const& value, TypeName, IInspectable const&, hstring const&) const
	{
		return box_value(static_cast<bool>(value));
	}

	IInspectable NullToBoolConverter::ConvertBack(IInspectable const&, TypeName, IInspectable const&, hstring const&) const
	{
		throw hresult_not_implemented();
	}

	IInspectable VisibilityToOpacityConverter::Convert(IInspectable const& value, TypeName, IInspectable const&, hstring const&) const
	{
		return box_value(unbox_value_or(value, Visibility::Visible) == Visibility::Visible ? 1.0 : 0.0);
	}

	IInspectable VisibilityToOpacityConverter::ConvertBack(IInspectable const&, TypeName, IInspectable const&, hstring const&) const
	{
		throw hresult_not_implemented();
	}

	IInspectable BoolReverseConverter::ConvertValue(IInspectable const& value, TypeName, IInspectable const&, hstring const&)
	{
		if (auto flag = value.try_as<bool>()) return box_value(!*flag);
		else return box_value(!value);
	}

	IInspectable StringTypeIdConverter::ConvertValue(IInspectable const& value, TypeName, IInspectable const&, hstring const&)
	{
		if (auto typeName = value.try_as<TypeName>()) return box_value(typeName->Name);
		else if (auto string = value.try_as<hstring>())
		{
			TypeName result;
			result.Name = *string;
			result.Kind = std::find(string->begin(), string->end(), L'.') == string->end() ? TypeKind::Primitive : TypeKind::Metadata;
			return box_value(result);
		}
		else throw hresult_invalid_argument(L"Value must be either a string or a type id.");
	}

	IInspectable StringVersionConverter::ConvertValue(IInspectable const& value, TypeName, IInspectable const&, hstring const&)
	{
		if (auto packageVersion = value.try_as<PackageVersion>()) return box_value(PackageVersionHelper::Stringify(*packageVersion));
		else if (auto string = value.try_as<hstring>()) return box_value(PackageVersionHelper::Parse(*string));
		else throw hresult_invalid_argument(L"Value must be either a string or a package version.");
	}

	IInspectable StringFileSizeConverter::ConvertValue(IInspectable const& value, TypeName targetType, IInspectable const&, hstring const&)
	{
		/* if (auto size = value.try_as<uint64>()) return box_value(FileSizeHelper::ToFormattedString(*size, m_UseBinaryPrefix));
		else if (auto string = value.try_as<hstring>()) return box_value(FileSizeHelper::FromFormattedString(*string));
		else return TypeDetails(targetType).CreateInstance(); */
		return {};
	}

	IInspectable StringLocalizationConverter::Convert(IInspectable const& value, TypeName, IInspectable const&, hstring const&) const
	{
		return box_value(m_Context ? m_Context.GetString(unbox_value_or(value, L"")) : L"");
	}

	IInspectable StringLocalizationConverter::ConvertBack(IInspectable const& value, TypeName targetType, IInspectable const& parameter, hstring const& language) const
	{
		throw hresult_not_implemented();
	}

	IInspectable StringToVisibilityConverter::Convert(IInspectable const& value, TypeName, IInspectable const&, hstring const&)
	{
		return box_value(!has_only_whitespaces(unbox_value_or(value, L"")) ? Visibility::Visible : Visibility::Collapsed);
	}

	IInspectable StringToVisibilityConverter::ConvertBack(IInspectable const&, TypeName, IInspectable const&, hstring const&)
	{
		throw hresult_not_implemented();
	}
}