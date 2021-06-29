#include "pch.h"
#include "ViewServiceUniversal.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::Core;

namespace winrt::OneToolkit::UI
{
	__interface __declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F")) ICoreWindowInterop : ::IUnknown
	{
		HRESULT __stdcall get_WindowHandle(HWND* hwnd) noexcept;
		HRESULT __stdcall put_MessageHandled(bool value) noexcept;
	};

	hstring ViewServiceUniversal::Title() const
	{
		return m_AppView.Title();
	}

	void ViewServiceUniversal::Title(hstring const& value)
	{
		if (m_AppView.Title() != value)
		{
			m_AppView.Title(value);
			Raise(L"Title");
		}
	}

	bool ViewServiceUniversal::IsFullScreen() const
	{
		return m_AppView.IsFullScreenMode();
	}

	void ViewServiceUniversal::IsFullScreen(bool value)
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

	bool ViewServiceUniversal::ExtendsContentIntoTitleBar() const
	{
		return m_CoreAppView.TitleBar().ExtendViewIntoTitleBar();
	}

	void ViewServiceUniversal::ExtendsContentIntoTitleBar(bool value)
	{
		if (m_CoreAppView.TitleBar().ExtendViewIntoTitleBar() != value)
		{
			m_CoreAppView.TitleBar().ExtendViewIntoTitleBar(value);
			Raise(L"ExtendViewIntoTitleBar");
		}
	}

	int64_t ViewServiceUniversal::WindowHandle() const
	{
		HWND windowHandle;
		check_hresult(m_CoreAppView.CoreWindow().as<ICoreWindowInterop>()->get_WindowHandle(&windowHandle));
		return juv::as_value<int64_t>(windowHandle);
	}

	Rect ViewServiceUniversal::Bounds() const
	{
		return m_CoreAppView.CoreWindow().Bounds();
	}

	IInspectable ViewServiceUniversal::ReferenceSource() const noexcept
	{
		return m_CoreAppView.CoreWindow();
	}

	void ViewServiceUniversal::SetMinimumSize(Size const& minSize) const
	{
		m_AppView.SetPreferredMinSize(minSize);
	}

	IAsyncOperation<bool> ViewServiceUniversal::CloseAsync() const
	{
		return m_AppView.TryConsolidateAsync();
	}
}