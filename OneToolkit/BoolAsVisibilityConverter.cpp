#include "pch.h"
#include "BoolAsVisibilityConverter.h"
#include "UI.Converters.BoolAsVisibilityConverter.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	Visibility BoolAsVisibilityConverter::VisibilityIfFalse() const noexcept
	{
		return VisibilityIfTrue() == Visibility::Visible ? Visibility::Collapsed : Visibility::Visible;
	}

	IInspectable BoolAsVisibilityConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return box_value(unbox_value<bool>(value) ? VisibilityIfTrue() : VisibilityIfFalse());
	}

	IInspectable BoolAsVisibilityConverter::ConvertBack(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return box_value(unbox_value<Visibility>(value) == VisibilityIfTrue());
	}

	void BoolAsVisibilityConverter::WhenPropertyChanged(PropertyChangedEventArgs const& args)
	{
		if (args.PropertyName() == L"VisibilityIfTrue") Raise(L"VisibilityIfFalse");
	}
}