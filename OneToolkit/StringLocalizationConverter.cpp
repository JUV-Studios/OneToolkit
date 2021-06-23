#include "pch.h"
#include "StringLocalizationConverter.h"
#include "UI.Converters.StringLocalizationConverter.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::UI::Converters::implementation
{
	IInspectable StringLocalizationConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&) const
	{
		return box_value(m_Context.GetString(unbox_value<hstring>(value)));
	}

	IInspectable StringLocalizationConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&) const
	{
		throw hresult_not_implemented();
	}
}