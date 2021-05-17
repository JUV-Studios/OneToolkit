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
    ResourceLoader StringLocalizationConverter::Context() const noexcept
    {
        return m_Context;
    }

    void StringLocalizationConverter::Context(ResourceLoader const& value)
    {
        if (m_Context != value)
        {
            m_Context = value;
            m_PropertyChanged(*this, PropertyChangedEventArgs(L"Context"));
        }
    }

    event_token StringLocalizationConverter::PropertyChanged(PropertyChangedEventHandler const& handler)
    {
        return m_PropertyChanged.add(handler);
    }

    void StringLocalizationConverter::PropertyChanged(event_token const& token) noexcept
    {
        m_PropertyChanged.remove(token);
    }

    IInspectable StringLocalizationConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
    {
        auto key = unbox_value<hstring>(value);
        auto translation = m_Context.GetString(key);
        return box_value(translation);
    }

    IInspectable StringLocalizationConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
    {
        throw hresult_not_implemented();
    }
}
