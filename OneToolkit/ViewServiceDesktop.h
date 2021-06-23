#pragma once
#include "ViewServiceBase.h"

namespace winrt::OneToolkit::UI
{
	struct ViewServiceDesktop : ViewServiceBase<ViewServiceDesktop>
	{
	public:
		ViewServiceDesktop(int64_t windowHandle);
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
		HWND m_WindowHandle;
		Runtime::DynamicModule user32 { L"User32.dll" };
		Runtime::DynamicModule advapi32 { L"Advapi32.dll" };
	};
}