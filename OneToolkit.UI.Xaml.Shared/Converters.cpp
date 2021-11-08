#include "pch.h"
#include "Converters.h"

using namespace juv;
using namespace Platform;
using namespace Framework;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::UI::Xaml::Interop;
using namespace OneToolkit::System;
using namespace OneToolkit::UI::Xaml::Converters;

define_dependency_property(ResourceLoader^, StringLocalizationConverter, Context, ResourceLoader::GetForViewIndependentUse());

Object^ BoolReverseConverter::ConvertValue(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	if (auto flag = dynamic_cast<IBox<bool>^>(value)) return flag->Value;
	return !value;
}

Object^ StringVersionConverter::ConvertValue(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	if (auto packageVersion = dynamic_cast<IBox<PackageVersion>^>(value)) return PackageVersionHelper::Stringify(packageVersion->Value);
	else return PackageVersionHelper::Parse(value->ToString());
}

Object^ StringLocalizationConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	return Context->GetString(value->ToString());
}

Object^ StringLocalizationConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	throw ref new NotImplementedException();
}

Object^ StringToVisibilityConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	return has_only_whitespaces(value->ToString()) ? Visibility::Collapsed : Visibility::Visible;
}

Object^ StringToVisibilityConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	throw ref new NotImplementedException();
}