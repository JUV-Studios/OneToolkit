#include "pch.h"
#include "BoolAsObjectConverter.h"
#include "UI.Converters.BoolAsObjectConverter.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	IInspectable BoolAsObjectConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return unbox_value<bool>(value) ? ValueForTrue() : ValueForFalse();
	}

	IInspectable BoolAsObjectConverter::ConvertBack(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return box_value(value == ValueForTrue());
	}
}