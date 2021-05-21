#pragma once
#include <winrt/OneToolkit.UI.h>

namespace winrt::OneToolkit::UI::implementation
{
    struct ViewServiceWin32 : implements<ViewServiceWin32, IViewServiceProvider, non_agile>
    {
    public:
        ViewServiceWin32(HWND windowHandle);
        hstring Title() const;
        void Title(hstring const& value);
        bool IsFullScreen() const;
        void IsFullScreen(bool value);
        IInspectable ReferenceSource() const noexcept;
        Windows::Foundation::IAsyncOperation<bool> CloseAsync();
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token token) noexcept;
    private:
        HWND m_WindowHandle;
        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
    };
}