#pragma once
#include <winrt/OneToolkit.UI.h>

namespace winrt::OneToolkit::UI::implementation
{
	struct ViewServiceUwp : implements<ViewServiceUwp, IViewServiceProvider, non_agile>
	{
    public:
        hstring Title() const;
        void Title(hstring const& value);
        bool IsFullScreen() const;
        void IsFullScreen(bool value);
        IInspectable ReferenceSource() const noexcept;
        Windows::Foundation::IAsyncOperation<bool> CloseAsync();
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token token) noexcept;
    private:
        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
        Windows::UI::ViewManagement::ApplicationView m_AppView = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        Windows::ApplicationModel::Core::CoreApplicationView m_CoreAppView = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView();
	};
}