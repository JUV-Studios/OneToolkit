#pragma once
#include "UI.ViewService.g.h"

namespace winrt::OneToolkit::UI
{
    namespace implementation
    {
        struct ViewServiceUwp : implements<ViewServiceUwp, IViewServiceProvider, non_agile>, Mvvm::ObservableBase<ViewServiceUwp>
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

        struct ViewServiceWin32 : implements<ViewServiceWin32, IViewServiceProvider, non_agile>, Mvvm::ObservableBase<ViewServiceWin32>
        {
        public:
            ViewServiceWin32(HWND windowHandle);
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
            HWND m_WindowHandle;
            juv::runtime::dynamic_module user32 { L"User32.dll" };
        };

        struct ViewService : ViewServiceT<ViewService>
        {
            ViewService() = delete;
            static IViewServiceProvider GetForCurrentView();
            static IViewServiceProvider GetForWindowId(int64_t windowHandle);
        };
    }
    
    namespace factory_implementation
    {
        struct ViewService : ViewServiceT<ViewService, implementation::ViewService>
        {
        };
    }
}
