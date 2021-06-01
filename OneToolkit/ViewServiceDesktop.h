#pragma once
#include <winrt/OneToolkit.UI.h>

namespace winrt::OneToolkit::UI
{
	struct ViewServiceDesktop : implements<ViewServiceDesktop, IViewServiceProvider, non_agile>, Mvvm::ObservableBase<ViewServiceDesktop>
	{
	public:
		ViewServiceDesktop(int64_t windowHandle);
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
}