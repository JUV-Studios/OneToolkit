#include "pch.h"
#include "OneToolkit.h"
#include "ViewService.h"
#include "UI.ViewService.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::UI::implementation
{
	__interface __declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F")) ICoreWindowInterop : ::IUnknown
	{
		HRESULT __stdcall get_WindowHandle(HWND* hwnd) noexcept;
		HRESULT __stdcall put_MessageHandled(bool value) noexcept;
	};

	hstring ViewServiceUwp::Title() const
	{
		return m_AppView.Title();
	}

	void ViewServiceUwp::Title(hstring const& value)
	{
		if (m_AppView.Title() != value)
		{
			m_AppView.Title(value);
			Raise(L"Title");
		}
	}

	bool ViewServiceUwp::IsFullScreen() const
	{
		return m_AppView.IsFullScreenMode();
	}

	void ViewServiceUwp::IsFullScreen(bool value)
	{
		if (m_AppView.IsFullScreenMode() != value)
		{
			if (value)
			{
				if (!m_AppView.TryEnterFullScreenMode()) return;
			}
			else m_AppView.ExitFullScreenMode();
			Raise(L"IsFullScreen");
		}
	}

	bool ViewServiceUwp::ExtendViewIntoTitleBar() const
	{
		return m_CoreAppView.TitleBar().ExtendViewIntoTitleBar();
	}

	void ViewServiceUwp::ExtendViewIntoTitleBar(bool value)
	{
		if (m_CoreAppView.TitleBar().ExtendViewIntoTitleBar() != value)
		{
			m_CoreAppView.TitleBar().ExtendViewIntoTitleBar(value);
			Raise(L"ExtendViewIntoTitleBar");
		}
	}

	int64_t ViewServiceUwp::WindowHandle() const
	{
		HWND result;
		check_hresult(m_CoreAppView.CoreWindow().as<ICoreWindowInterop>()->get_WindowHandle(&result));
		return juv::as_value<int64_t>(result);
	}

	IInspectable ViewServiceUwp::ReferenceSource() const noexcept
	{
		return m_CoreAppView;
	}

	IAsyncOperation<bool> ViewServiceUwp::CloseAsync() const
	{
		return m_AppView.TryConsolidateAsync();
	}

	typedef int(__stdcall* GetWindowTextW)(HWND, LPWSTR, int);

	typedef int(__stdcall* SetWindowTextW)(HWND, LPCWSTR);

	typedef int(__stdcall* DestroyWindow)(HWND);

	ViewServiceWin32::ViewServiceWin32(HWND windowHandle) : m_WindowHandle(windowHandle)
	{
	}

	hstring ViewServiceWin32::Title() const
	{
		auto func = user32.get_proc_address<GetWindowTextW>("GetWindowTextW");
		std::array<wchar_t, MAX_PATH> result;
		check_bool(func(m_WindowHandle, result.data(), MAX_PATH));
		return result.data();
	}

	void ViewServiceWin32::Title(hstring const& value)
	{
		if (Title() != value)
		{
			auto func = user32.get_proc_address<SetWindowTextW>("SetWindowTextW");
			check_bool(func(m_WindowHandle, value.data()));
			Raise(L"Title");
		}
	}

	bool ViewServiceWin32::IsFullScreen() const
	{
		return false;
	}

	void ViewServiceWin32::IsFullScreen(bool value)
	{
	}

	bool ViewServiceWin32::ExtendViewIntoTitleBar() const
	{
		return false;
	}

	void ViewServiceWin32::ExtendViewIntoTitleBar(bool value)
	{
	}

	int64_t ViewServiceWin32::WindowHandle() const
	{
		return juv::as_value<int64_t>(m_WindowHandle);
	}

	IInspectable ViewServiceWin32::ReferenceSource() const noexcept
	{
		return box_value(WindowHandle());
	}

	IAsyncOperation<bool> ViewServiceWin32::CloseAsync() const
	{
		auto func = user32.get_proc_address<DestroyWindow>("DestroyWindow");
		co_return func(m_WindowHandle);
	}

	IViewServiceProvider ViewService::GetForCurrentView()
	{
		return make<ViewServiceUwp>();
	}

	IViewServiceProvider ViewService::GetForWindowId(int64_t windowHandle)
	{
		return make<ViewServiceWin32>(juv::as_pointer<HWND>(windowHandle));
	}
}