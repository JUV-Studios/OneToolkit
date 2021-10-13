#include "pch.h"
#include "ViewService.h"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;
using namespace OneToolkit::Mvvm;
using namespace OneToolkit::System;
using namespace OneToolkit::Runtime;

void* winrt_make_OneToolkit_UI_ViewService()
{
	return detach_abi(make<OneToolkit::UI::factory_implementation::ViewService>());
}

namespace winrt::OneToolkit::UI
{
	using RegGetValueW = int(__stdcall*)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);

	using GetWindowTextLengthW = int(__stdcall*)(HWND);

	using GetWindowTextW = int(__stdcall*)(HWND, LPWSTR, int);

	using SetWindowTextW = int(__stdcall*)(HWND, LPCWSTR);

	using GetWindowLongW = int(__stdcall*)(HWND, int);

	using DestroyWindow = int(__stdcall*)(HWND);

	using GetWindowRect = int(__stdcall*)(HWND, LPRECT);

	using GetDpiForWindow = uint32(__stdcall*)(HWND);

	using GetSystemMetricsForDpi = int(__stdcall*)(int, uint32);

	__interface __declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F")) ICoreWindowInterop : ::IUnknown
	{
		int __stdcall get_WindowHandle(HWND* hwnd) noexcept;
		int __stdcall put_MessageHandled(bool value) noexcept;
	};

	template <typename Derived>
	struct ViewServiceBase : implementation::ViewServiceT<Derived, non_agile>, ObservableBase<Derived>
	{
		void ToggleFullScreen() const
		{
			auto target = static_cast<Derived const*>(this);
			target->IsFullScreen(!target->IsFullScreen());
		}

		void CloseView() const
		{
			static_cast<Derived const*>(this)->TryCloseAsync();
		}
	};

	struct ViewServiceUniversal : ViewServiceBase<ViewServiceUniversal>
	{
		ViewServiceUniversal()
		{
			m_CoreAppView.CoreWindow().SizeChanged([weakThis = get_weak()](CoreWindow, WindowSizeChangedEventArgs)
			{
				if (auto strongThis = weakThis.get())
				{
					strongThis->Raise(L"Bounds");
					strongThis->Raise(L"IsFullScreen");
				}
			});

			m_CoreAppView.TitleBar().LayoutMetricsChanged([weakThis = get_weak()](CoreApplicationViewTitleBar, IInspectable)
			{
				if (auto strongThis = weakThis.get())
				{
					strongThis->Raise(L"TitleBarInset");
					strongThis->Raise(L"TitleBarHeight");
				}
			});

			/*m_CoreAppView.TitleBar().IsVisibleChanged([weakThis = get_weak()](CoreApplicationViewTitleBar, IInspectable)
			{
				if (auto strongThis = weakThis.get())
				{
					strongThis->Raise(L"TitleBarHeight");
				}
			});*/
		}

		DeclareAutoProperty(ApplicationView, AppView, ApplicationView::GetForCurrentView());

		DeclareAutoProperty(CoreApplicationView, CoreAppView, CoreApplication::GetCurrentView());

		DeclareAutoProperty(SystemNavigationManager, NavigationManager, SystemNavigationManager::GetForCurrentView());

		hstring Title() const
		{
			return m_AppView.Title();
		}

		void Title(hstring const& value)
		{
			m_AppView.Title(value);
		}

		WindowId Id() const
		{
			return factory_implementation::ViewService::GetCoreWindowId(m_CoreAppView.CoreWindow());
		}

		bool IsFullScreen() const
		{
			return m_AppView.IsFullScreenMode();
		}

		void IsFullScreen(bool value) const
		{
			if (m_AppView.IsFullScreenMode() != value)
			{
				if (value) m_AppView.TryEnterFullScreenMode();
				else m_AppView.ExitFullScreenMode();
			}
		}

		Rect Bounds() const
		{
			return m_CoreAppView.CoreWindow().Bounds();
		}

		double TitleBarHeight() const
		{
			return m_CoreAppView.TitleBar().Height();
		}

		Thickness TitleBarInset() const
		{
			return { m_CoreAppView.TitleBar().SystemOverlayLeftInset(), 0, m_CoreAppView.TitleBar().SystemOverlayRightInset(), 0 };
		}

		IAsyncOperation<bool> TryCloseAsync() const
		{
			return m_AppView.TryConsolidateAsync();
		}
	};

	struct ViewServiceDesktop : ViewServiceBase<ViewServiceDesktop>
	{
	public:
		ViewServiceDesktop(WindowId windowHandle)
		{
			m_WindowHandle = as_pointer<HWND>(windowHandle.Value);
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
			check_bool(user32.GetProcAddress<SetWindowTextW>("SetWindowTextW")(m_WindowHandle, value.data()));
		}

		WindowId Id() const
		{
			return { as_value<uint64>(m_WindowHandle) };
		}

		bool IsFullScreen() const
		{
			auto style = user32.GetProcAddress<GetWindowLongW>("GetWindowLongW")(m_WindowHandle, GWL_STYLE);
			return !(style & WS_OVERLAPPEDWINDOW);
		}

		void IsFullScreen(bool value) const
		{
			if (IsFullScreen() != value)
			{
				if (value)
				{
				}
				else
				{

				}
			}

		}

		Rect Bounds() const
		{
			RECT result{};
			check_bool(user32.GetProcAddress<GetWindowRect>("GetWindowRect")(m_WindowHandle, &result));
			float width = static_cast<float>(result.right - result.left);
			float height = static_cast<float>(result.bottom - result.top);
			return { static_cast<float>(result.left), static_cast<float>(result.top), width, height };
		}

		double TitleBarHeight() const
		{
			auto const dpi = user32.GetProcAddress<GetDpiForWindow>("GetDpiForWindow")(m_WindowHandle);
			auto const func = user32.GetProcAddress<GetSystemMetricsForDpi>("GetSystemMetricsForDpi");
			return func(SM_CYFRAME, dpi) + func(SM_CYCAPTION, dpi) + func(SM_CXPADDEDBORDER, dpi);
		}

		Thickness TitleBarInset() const
		{
			return {};
		}

		ApplicationView AppView() const noexcept
		{
			return nullptr;
		}

		CoreApplicationView CoreAppView() const noexcept
		{
			return nullptr;
		}

		SystemNavigationManager NavigationManager() const noexcept
		{
			return nullptr;
		}

		IAsyncOperation<bool> TryCloseAsync() const
		{
			co_return user32.GetProcAddress<DestroyWindow>("DestroyWindow")(m_WindowHandle) != false;
		}
	private:
		HWND m_WindowHandle;
		DynamicLibrary user32{ L"User32.dll" };
	};

	namespace factory_implementation
	{
		bool ViewService::IsFreeWindowingSupported()
		{
			auto deviceFamily = MachineInformation::DeviceFamily();
			return (deviceFamily == L"Windows.Desktop" || deviceFamily == L"Windows.Holographic") && InteractionMode() != UserInteractionMode::Touch;
		}

		UserInteractionMode ViewService::InteractionMode()
		{
			if (CoreApplication::Views().Size()) return UIViewSettings::GetForCurrentView().UserInteractionMode();
			else
			{
				unsigned long result = 0;
				unsigned long size = sizeof(result);
				DynamicLibrary advapi32{ L"Advapi32.dll" };
				check_win32(advapi32.GetProcAddress<RegGetValueW>("RegGetValueW")(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", L"TabletMode",
					RRF_RT_REG_DWORD, nullptr, &result, &size));
				return static_cast<UserInteractionMode>(result);
			}
		}

		OneToolkit::UI::ViewService ViewService::GetForCurrentView()
		{
			return make<ViewServiceUniversal>();
		}

		OneToolkit::UI::ViewService ViewService::GetForWindowId(WindowId windowId)
		{
			if (CoreApplication::Views().Size()) throw hresult_illegal_method_call(L"GetForWindowId is for traditional desktop apps only. Universal apps should call GetForCurrentView instead.");
			return make<ViewServiceDesktop>(windowId);
		}

		WindowId ViewService::GetCoreWindowId(CoreWindow const& coreWindow)
		{
			HWND windowHandle;
			check_hresult(coreWindow.as<ICoreWindowInterop>()->get_WindowHandle(&windowHandle));
			return { as_value<uint64>(windowHandle) };
		}
	}
}