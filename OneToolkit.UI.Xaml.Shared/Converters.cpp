#include "pch.h"
#include "Converters.h"

using namespace juv;
using namespace Platform;
using namespace AppFramework;
using namespace Windows::UI::Xaml::Interop;
using namespace OneToolkit::Data::Text;
using namespace OneToolkit::UI::Xaml::Converters;

define_dependency_property(ResourceLoader^, StringLocalizationConverter, Context, GetDefaultContext());

define_dependency_property(Visibility, NullToVisibilityConverter, ValueForNull, Visibility::Collapsed);

Object^ StringLocalizationConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	return Context->GetString(value->ToString());
}

Object^ StringLocalizationConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	throw ref new NotImplementedException();
}

ResourceLoader^ StringLocalizationConverter::GetDefaultContext()
{
#ifdef framework_winui3
	return ref new ResourceLoader();
#else
	return ResourceLoader::GetForViewIndependentUse();
#endif
}

Object^ NullToVisibilityConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	return !value ? ValueForNull : (ValueForNull == Visibility::Collapsed ? Visibility::Visible : Visibility::Collapsed);
}

Object^ NullToVisibilityConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	throw ref new NotImplementedException();
}

Object^ StringToVisibilityConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	return StringExtensions::HasOnlyWhitespaces(value->ToString()) ? Visibility::Collapsed : Visibility::Visible;
}

Object^ StringToVisibilityConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	throw ref new NotImplementedException();
}