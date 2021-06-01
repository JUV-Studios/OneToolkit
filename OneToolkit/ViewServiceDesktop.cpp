#include "pch.h"
#include "ViewServiceDesktop.h"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::UI
{
	typedef int(__stdcall* GetWindowTextW)(HWND, LPWSTR, int);

	typedef int(__stdcall* SetWindowTextW)(HWND, LPCWSTR);

	typedef int(__stdcall* DestroyWindow)(HWND);

	ViewServiceDesktop::ViewServiceDesktop(int64_t windowHandle)
	{
		m_WindowHandle = juv::as_pointer<HWND>(windowHandle);
	}

	hstring ViewServiceDesktop::Title() const
	{
		auto func = user32.get_proc_address<GetWindowTextW>("GetWindowTextW");
		wchar_t result[MAX_PATH];
		check_bool(func(m_WindowHandle, result, MAX_PATH));
		return result;
	}

	void ViewServiceDesktop::Title(hstring const& value)
	{
		if (Title() != value)
		{
			auto func = user32.get_proc_address<SetWindowTextW>("SetWindowTextW");
			check_bool(func(m_WindowHandle, value.data()));
			Raise(L"Title");
		}
	}

	bool ViewServiceDesktop::IsFullScreen() const
	{
		return false;
	}

	void ViewServiceDesktop::IsFullScreen(bool value)
	{
	}

	bool ViewServiceDesktop::ExtendViewIntoTitleBar() const
	{
		return false;
	}

	void ViewServiceDesktop::ExtendViewIntoTitleBar(bool value)
	{
	}

	int64_t ViewServiceDesktop::WindowHandle() const
	{
		return juv::as_value<int64_t>(m_WindowHandle);
	}

	IInspectable ViewServiceDesktop::ReferenceSource() const noexcept
	{
		return box_value(WindowHandle());
	}

	IAsyncOperation<bool> ViewServiceDesktop::CloseAsync() const
	{
		auto func = user32.get_proc_address<DestroyWindow>("DestroyWindow");
		co_return func(m_WindowHandle) == 1;
	}
}