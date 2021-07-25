#include "pch.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace OneToolkit::UI::Xaml::Converters
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct StringToTypeConverter sealed : IValueConverter
	{
		virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
		{
			TypeName result;
			result.Name = value->ToString();
			result.Kind = TypeKind::Metadata;
			return result;
		}

		virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
		{
			return static_cast<TypeName>(value).Name;
		}
	};
}