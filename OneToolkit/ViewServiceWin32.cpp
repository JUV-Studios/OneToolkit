#include "pch.h"
#include "OneToolkit.h"
#include "ViewServiceWin32.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;

namespace winrt::OneToolkit::UI::implementation
{
	extern "C" int GetWindowTextW(HWND hWnd, LPWSTR lpString, int nMaxCount);

	extern "C" BOOL SetWindowTextW(HWND hWnd, LPCWSTR lpString);

	extern "C" BOOL DestroyWindow(HWND hWnd);

	ViewServiceWin32::ViewServiceWin32(HWND windowHandle) : m_WindowHandle(windowHandle)
	{
	}

	hstring ViewServiceWin32::Title() const
	{
		std::array<wchar_t, 64> value;
		if (GetWindowTextW(m_WindowHandle, value.data(), static_cast<int>(value.size())) == 0) throw_last_error();
		return value.data();
	}

	void ViewServiceWin32::Title(hstring const& value)
	{
		if (Title() != value)
		{
			SetWindowTextW(m_WindowHandle, value.data());
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"Title"));
		}
	}

	bool ViewServiceWin32::IsFullScreen() const
	{
		throw hresult_not_implemented();
	}

	void ViewServiceWin32::IsFullScreen(bool value)
	{
		throw hresult_not_implemented();
	}

	IInspectable ViewServiceWin32::ReferenceSource() const noexcept
	{
		auto handleValue = juv::as_value<uint64_t>(m_WindowHandle);
		return box_value(handleValue);
	}

	IAsyncOperation<bool> ViewServiceWin32::CloseAsync()
	{
		co_return DestroyWindow(m_WindowHandle) == TRUE;
	}

	event_token ViewServiceWin32::PropertyChanged(PropertyChangedEventHandler const& handler)
	{
		return m_PropertyChanged.add(handler);
	}

	void ViewServiceWin32::PropertyChanged(event_token token) noexcept
	{
		m_PropertyChanged.remove(token);
	}
}