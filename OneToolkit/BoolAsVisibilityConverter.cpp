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
	Visibility BoolAsVisibilityConverter::VisibilityIfTrue() const noexcept
	{
		return m_VisibilityIfTrue;
	}

	void BoolAsVisibilityConverter::VisibilityIfTrue(Visibility value)
	{
		if (m_VisibilityIfTrue != value)
		{
			m_VisibilityIfTrue = value;
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"VisibilityIfTrue"));
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"VisibilityIfFalse"));
		}
	}

	Visibility BoolAsVisibilityConverter::VisibilityIfFalse() const noexcept
	{
		return m_VisibilityIfTrue == Visibility::Visible ? Visibility::Collapsed : Visibility::Visible;
	}

	IInspectable BoolAsVisibilityConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return box_value(unbox_value<bool>(value) ? VisibilityIfTrue() : VisibilityIfFalse());
	}

	IInspectable BoolAsVisibilityConverter::ConvertBack(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return box_value(unbox_value<Visibility>(value) == VisibilityIfTrue());
	}

	event_token BoolAsVisibilityConverter::PropertyChanged(PropertyChangedEventHandler const& handler)
	{
		return m_PropertyChanged.add(handler);
	}

	void BoolAsVisibilityConverter::PropertyChanged(event_token token) noexcept
	{
		m_PropertyChanged.remove(token);
	}
}