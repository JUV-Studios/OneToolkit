#include "pch.h"
#include "BoolAsObjectConverter.h"
#include "UI.Converters.BoolAsObjectConverter.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	IInspectable BoolAsObjectConverter::ValueForTrue() const noexcept
	{
		return m_TrueValue;
	}

	void BoolAsObjectConverter::ValueForTrue(IInspectable const& value)
	{
		if (m_TrueValue != value)
		{
			m_TrueValue = value;
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"ValueForTrue"));
		}
	}

	IInspectable BoolAsObjectConverter::ValueForFalse() const noexcept
	{
		return m_FalseValue;
	}

	void BoolAsObjectConverter::ValueForFalse(IInspectable const& value)
	{
		if (m_FalseValue != value)
		{
			m_FalseValue = value;
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"ValueForFalse"));
		}
	}

	IInspectable BoolAsObjectConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return unbox_value<bool>(value) ? m_TrueValue : m_FalseValue;
	}

	IInspectable BoolAsObjectConverter::ConvertBack(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		return box_value(value == m_TrueValue);
	}

	event_token BoolAsObjectConverter::PropertyChanged(PropertyChangedEventHandler const& handler)
	{
		return m_PropertyChanged.add(handler);
	}

	void BoolAsObjectConverter::PropertyChanged(event_token token) noexcept
	{
		m_PropertyChanged.remove(token);
	}
}