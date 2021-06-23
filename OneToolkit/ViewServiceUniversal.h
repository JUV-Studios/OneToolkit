#pragma once
#include "ViewServiceBase.h"

namespace winrt::OneToolkit::UI
{
	struct ViewServiceUniversal : ViewServiceBase<ViewServiceUniversal>
	{
    public:
        hstring Title() const;
        void Title(hstring const& value);
        bool IsFullScreen() const;
        void IsFullScreen(bool value);
        bool ExtendsContentIntoTitleBar() const;
        void ExtendsContentIntoTitleBar(bool value);
        int64_t WindowHandle() const;
        Windows::Foundation::Rect Bounds() const;
        IInspectable ReferenceSource() const noexcept;
        Windows::UI::ViewManagement::UserInteractionMode InteractionMode() const;
        void SetMinimumSize(Windows::Foundation::Size const& minSize) const;
        Windows::Foundation::IAsyncOperation<bool> CloseAsync() const;
    private:
        Windows::UI::ViewManagement::ApplicationView m_AppView = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        Windows::UI::ViewManagement::UIViewSettings m_ViewSettings = Windows::UI::ViewManagement::UIViewSettings::GetForCurrentView();
        Windows::ApplicationModel::Core::CoreApplicationView m_CoreAppView = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView();
	};
}