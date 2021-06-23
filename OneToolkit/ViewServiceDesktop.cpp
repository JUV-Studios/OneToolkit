#include "pch.h"
#include "ViewServiceDesktop.h"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::ViewManagement;

namespace winrt::OneToolkit::UI
{
	typedef int(__stdcall* GetWindowTextLengthW)(HWND);

	typedef int(__stdcall* GetWindowTextW)(HWND, LPWSTR, int);

	typedef int(__stdcall* SetWindowTextW)(HWND, LPCWSTR);

	typedef int(__stdcall* DestroyWindow)(HWND);

	typedef int(__stdcall* GetWindowRect)(HWND, LPRECT);

	typedef int(__stdcall* RegGetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);

	ViewServiceDesktop::ViewServiceDesktop(int64_t windowHandle)
	{
		m_WindowHandle = juv::as_pointer<HWND>(windowHandle);
	}

	hstring ViewServiceDesktop::Title() const
	{
		auto titleSize = static_cast<uint32>(user32.GetProcAddress<GetWindowTextLengthW>("GetWindowTextLengthW")(m_WindowHandle) + 1);
		impl::hstring_builder stringBuilder { titleSize };
		check_bool(user32.GetProcAddress<GetWindowTextW>("GetWindowTextW")(m_WindowHandle, stringBuilder.data(), titleSize));
		return stringBuilder.to_hstring();
	}

	void ViewServiceDesktop::Title(hstring const& value)
	{
		if (Title() != value)
		{
			check_bool(user32.GetProcAddress<SetWindowTextW>("SetWindowTextW")(m_WindowHandle, value.data()));
			Raise(L"Title");
		}
	}

	bool ViewServiceDesktop::IsFullScreen() const
	{
		return false;
	}

	void ViewServiceDesktop::IsFullScreen(bool)
	{
		throw hresult_not_implemented();
	}

	bool ViewServiceDesktop::ExtendsContentIntoTitleBar() const
	{
		throw hresult_not_implemented();
	}

	void ViewServiceDesktop::ExtendsContentIntoTitleBar(bool)
	{
		throw hresult_not_implemented();
	}

	int64_t ViewServiceDesktop::WindowHandle() const
	{
		return juv::as_value<int64_t>(m_WindowHandle);
	}

	Rect ViewServiceDesktop::Bounds() const
	{
		RECT result;
		check_bool(user32.GetProcAddress<GetWindowRect>("GetWindowRect")(m_WindowHandle, &result));
		float width = static_cast<float>(result.right - result.left);
		float height = static_cast<float>(result.bottom - result.top);
		return { static_cast<float>(result.left), static_cast<float>(result.top), width, height };
	}

	IInspectable ViewServiceDesktop::ReferenceSource() const noexcept
	{
		return box_value(WindowHandle());
	}

	UserInteractionMode ViewServiceDesktop::InteractionMode() const
	{
		unsigned long result;
		unsigned long size = sizeof(result);
		check_win32(advapi32.GetProcAddress<RegGetValueW>("RegGetValueW")(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", L"TabletMode", RRF_RT_REG_DWORD, nullptr, &result, &size));
		return static_cast<UserInteractionMode>(result);
	}

	void ViewServiceDesktop::SetMinimumSize(Size const&) const
	{
		throw hresult_not_implemented();
	}

	IAsyncOperation<bool> ViewServiceDesktop::CloseAsync() const
	{
		co_return user32.GetProcAddress<DestroyWindow>("DestroyWindow")(m_WindowHandle) == 1;
	}
}