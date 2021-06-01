#pragma once
#include <winrt/OneToolkit.UI.h>

namespace winrt::OneToolkit::UI
{
	struct ViewServiceUniversal : implements<ViewServiceUniversal, IViewServiceProvider, non_agile>, Mvvm::ObservableBase<ViewServiceUniversal>
	{
    public:
        hstring Title() const;
        void Title(hstring const& value);
        bool IsFullScreen() const;
        void IsFullScreen(bool value);
        bool ExtendViewIntoTitleBar() const;
        void ExtendViewIntoTitleBar(bool value);
        int64_t WindowHandle() const;
        IInspectable ReferenceSource() const noexcept;
        Windows::Foundation::IAsyncOperation<bool> CloseAsync() const;
    private:
        Windows::UI::ViewManagement::ApplicationView m_AppView = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        Windows::ApplicationModel::Core::CoreApplicationView m_CoreAppView = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView();
	};
}