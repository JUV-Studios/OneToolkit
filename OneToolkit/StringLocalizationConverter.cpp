#include "pch.h"
#include "StringLocalizationConverter.h"
#include "UI.Converters.StringLocalizationConverter.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::ApplicationModel::Resources;

namespace winrt::OneToolkit::UI::Converters::implementation
{
    IInspectable StringLocalizationConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
    {
        auto key = unbox_value<hstring>(value);
        auto translation = Context().GetString(key);
        return box_value(translation);
    }

    IInspectable StringLocalizationConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
    {
        throw hresult_not_implemented();
    }
}