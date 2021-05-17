#include "pch.h"
#include "BoolReverseConverter.h"
#include "UI.Converters.BoolReverseConverter.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	IInspectable BoolReverseConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&) const noexcept
	{
		return box_value(!unbox_value<bool>(value));
	}

	IInspectable BoolReverseConverter::ConvertBack(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&) const noexcept
	{
		return box_value(!unbox_value<bool>(value));
	}
}
