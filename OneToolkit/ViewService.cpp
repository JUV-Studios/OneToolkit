#include "pch.h"
#include "ViewService.h"
#include "UI.ViewService.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;
using namespace OneToolkit::Mvvm;
using namespace OneToolkit::Runtime;
using namespace OneToolkit::ApplicationModel;

namespace winrt::OneToolkit::UI::implementation
{
	using RegGetValueW = int(__stdcall*)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);

	using GetWindowTextLengthW = int(__stdcall*)(HWND);

	using GetWindowTextW = int(__stdcall*)(HWND, LPWSTR, int);

	using SetWindowTextW = int(__stdcall*)(HWND, LPCWSTR);

	using DestroyWindow = int(__stdcall*)(HWND);

	using GetWindowRect = int(__stdcall*)(HWND, LPRECT);

	__interface __declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F")) ICoreWindowInterop : ::IUnknown
	{
		HRESULT __stdcall get_WindowHandle(HWND* hwnd) noexcept;
		HRESULT __stdcall put_MessageHandled(bool value) noexcept;
	};

	template <typename Derived>
	struct ViewServiceBase : implements<Derived, IViewServiceProvider, non_agile>
	{
	public:
		bool IsDialogShown()
		{
			return unbox_value_or(CoreApplication::Properties().TryLookup(DialogShownKey()), false);
		}

		void IsDialogShown(bool value)
		{
			if (IsDialogShown() != value)
			{
				static slim_mutex dialogShownLock;
				const slim_lock_guard lockGuard{ dialogShownLock };
				CoreApplication::Properties().Insert(DialogShownKey(), box_value(value));
			}
		}
	private:
		std::wstring m_DialogShownKey;

		inline std::wstring& DialogShownKey()
		{
			if (m_DialogShownKey.empty()) m_DialogShownKey = L"IsDialogShown-" + std::to_wstring(static_cast<Derived*>(this)->WindowHandle());
			return m_DialogShownKey;
		}
	};

	struct ViewServiceUniversal : ViewServiceBase<ViewServiceUniversal>
	{
	public:
		hstring Title() const
		{
			return m_AppView.Title();
		}

		void Title(hstring const& value)
		{
			m_AppView.Title(value);
		}

		int64_t WindowHandle() const
		{
			HWND windowHandle;
			check_hresult(m_CoreAppView.CoreWindow().as<ICoreWindowInterop>()->get_WindowHandle(&windowHandle));
			return as_value<int64_t>(windowHandle);
		}

		Rect Bounds() const
		{
			return m_CoreAppView.CoreWindow().Bounds();
		}

		IInspectable ReferenceSource() const noexcept
		{
			return m_CoreAppView.CoreWindow();
		}

		IAsyncOperation<bool> CloseAsync() const
		{
			return m_AppView.TryConsolidateAsync();
		}
	private:
		ApplicationView m_AppView = ApplicationView::GetForCurrentView();
		CoreApplicationView m_CoreAppView = CoreApplication::GetCurrentView();
	};

	struct ViewServiceDesktop : ViewServiceBase<ViewServiceDesktop>
	{
	public:
		ViewServiceDesktop(int64_t windowHandle)
		{
			m_WindowHandle = as_pointer<HWND>(windowHandle);
		}

		hstring Title() const
		{
			auto titleSize = static_cast<uint32>(user32.GetProcAddress<GetWindowTextLengthW>("GetWindowTextLengthW")(m_WindowHandle) + 1);
			impl::hstring_builder stringBuilder{ titleSize };
			check_bool(user32.GetProcAddress<GetWindowTextW>("GetWindowTextW")(m_WindowHandle, stringBuilder.data(), titleSize));
			return stringBuilder.to_hstring();
		}

		void Title(hstring const& value)
		{
			if (Title() != value) check_bool(user32.GetProcAddress<SetWindowTextW>("SetWindowTextW")(m_WindowHandle, value.data()));
		}

		int64_t WindowHandle() const
		{
			return as_value<int64_t>(m_WindowHandle);
		}

		Rect Bounds() const
		{
			RECT result{};
			check_bool(user32.GetProcAddress<GetWindowRect>("GetWindowRect")(m_WindowHandle, &result));
			float width = static_cast<float>(result.right - result.left);
			float height = static_cast<float>(result.bottom - result.top);
			return { static_cast<float>(result.left), static_cast<float>(result.top), width, height };
		}

		IInspectable ReferenceSource() const noexcept
		{
			return box_value(WindowHandle());
		}

		IAsyncOperation<bool> CloseAsync() const
		{
			co_return user32.GetProcAddress<DestroyWindow>("DestroyWindow")(m_WindowHandle) != false;
		}
	private:
		HWND m_WindowHandle;
		DynamicModule user32{ L"User32.dll" };
	};


	UserInteractionMode ViewService::InteractionMode()
	{
		if (AppInformation::IsCoreApplication()) return UIViewSettings::GetForCurrentView().UserInteractionMode();
		else
		{
			unsigned long result = 0;
			unsigned long size = sizeof(result);
			DynamicModule advapi32{ L"Advapi32.dll" };
			check_win32(advapi32.GetProcAddress<RegGetValueW>("RegGetValueW")(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", L"TabletMode", 
				RRF_RT_REG_DWORD, nullptr, &result, &size));
			return static_cast<UserInteractionMode>(result);
		}
	}

	IViewServiceProvider ViewService::GetForCurrentView()
	{
		return make<ViewServiceUniversal>();
	}

	IViewServiceProvider ViewService::GetForWindowId(int64_t windowHandle)
	{
		if (AppInformation::IsCoreApplication()) throw hresult_illegal_method_call(L"GetForWindowId must be invoked from desktop apps only. UWP apps should invoke GetForCurrentView instead.");
		return make<ViewServiceDesktop>(windowHandle);
	}
}