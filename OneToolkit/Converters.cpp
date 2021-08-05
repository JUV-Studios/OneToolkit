#include "pch.h"
#include "Converters.h"
#include "UI.Converters.StringTypeConverter.g.cpp"
#include "UI.Converters.StringVersionConverter.g.cpp"
#include "UI.Converters.StringFileSizeConverter.g.cpp"
#include "UI.Converters.StringLocalizationConverter.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace OneToolkit::Storage;
using namespace OneToolkit::Runtime;
using namespace OneToolkit::ApplicationModel;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	IInspectable StringTypeConverter::ConvertValue(IInspectable const& value, TypeName targetType, IInspectable const&, hstring const&)
	{
		if (auto typeName = value.try_as<TypeName>()) return box_value(typeName->Name);
		else if (auto string = value.try_as<hstring>()) return box_value(TypeNameHelper::FromString(*string));
		else return nullptr;
	}

	IInspectable StringVersionConverter::ConvertValue(IInspectable const& value, TypeName targetType, IInspectable const&, hstring const&)
	{
		if (auto packageVersion = value.try_as<PackageVersion>()) return box_value(PackageVersionHelper::ToFormattedString(*packageVersion));
		else if (auto string = value.try_as<hstring>()) return box_value(PackageVersionHelper::FromFormattedString(*string));
		else return nullptr;
	}

	IInspectable StringFileSizeConverter::ConvertValue(IInspectable const& value, TypeName targetType, IInspectable const&, hstring const&)
	{
		if (auto size = value.try_as<uint64>()) return box_value(FileSizeHelper::ToFormattedString(*size, m_UseBinaryPrefix));
		else if (auto string = value.try_as<hstring>()) return box_value(FileSizeHelper::FromFormattedString(*string, m_UseBinaryPrefix));
		else return nullptr;
	}


	IInspectable StringLocalizationConverter::ConvertValue(IInspectable const& value, TypeName targetType, IInspectable const&, hstring const&)
	{
		return box_value(m_Context ? m_Context.GetString(unbox_value_or(value, L"")) : L"");
	}
}