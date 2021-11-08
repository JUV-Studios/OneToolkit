#include "pch.h"
#include "ViewService.h"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;
using namespace OneToolkit::System;
using namespace OneToolkit::Runtime;
using namespace OneToolkit::Data::Mvvm;

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
	public:
		ViewServiceBase(WindowHandle windowHandle): m_Handle(as_pointer<HWND>(windowHandle.Value))
		{
		}

		WindowHandle Handle() const noexcept
		{
			return { as_value<uint64>(m_Handle) };
		}

		void ToggleFullScreen() const
		{
			auto target = static_cast<Derived const* const>(this);
			target->IsFullScreen(!target->IsFullScreen());
		}

		~ViewServiceBase()
		{
		}
	protected:
		HWND m_Handle;
	};

	struct ViewServiceUniversal : ViewServiceBase<ViewServiceUniversal>
	{
		ViewServiceUniversal() : ViewServiceBase<ViewServiceUniversal>({ 0 })
		{
			CoreAppView().CoreWindow().SizeChanged([weakThis = get_weak()](CoreWindow, WindowSizeChangedEventArgs)
			{
				if (auto strongThis = weakThis.get())
				{
					strongThis->Raise(L"Bounds");
					strongThis->Raise(L"IsFullScreen");
				}
			});

			CoreAppView().TitleBar().LayoutMetricsChanged([weakThis = get_weak()](CoreApplicationViewTitleBar, IInspectable)
			{
				if (auto strongThis = weakThis.get())
				{
					strongThis->Raise(L"TitleBarInset");
					strongThis->Raise(L"TitleBarHeight");
				}
			});

			CoreAppView().TitleBar().IsVisibleChanged([weakThis = get_weak()](CoreApplicationViewTitleBar, IInspectable)
			{
				if (auto strongThis = weakThis.get())
				{
					strongThis->Raise(L"TitleBarVisibility");
				}
			});
		}

		auto_property<ApplicationView> const AppView{ ApplicationView::GetForCurrentView() };
		
		auto_property<CoreApplicationView> const CoreAppView{ CoreApplication::GetCurrentView() };

		auto_property<SystemNavigationManager> const NavigationManager{ SystemNavigationManager::GetForCurrentView() };

		hstring Title() const
		{
			return AppView().Title();
		}

		void Title(hstring const& value)
		{
			AppView().Title(value);
		}

		bool IsFullScreen() const
		{
			return AppView().IsFullScreenMode();
		}

		void IsFullScreen(bool value) const
		{
			if (AppView().IsFullScreenMode() != value)
			{
				if (value) AppView().TryEnterFullScreenMode();
				else AppView().ExitFullScreenMode();
			}
		}

		Rect Bounds() const
		{
			return CoreAppView().CoreWindow().Bounds();
		}

		double TitleBarHeight() const
		{
			return CoreAppView().TitleBar().Height();
		}

		Visibility TitleBarVisibility() const
		{
			return CoreAppView().TitleBar().IsVisible() ? Visibility::Visible : Visibility::Collapsed;
		}

		Thickness TitleBarInset() const
		{
			return { CoreAppView().TitleBar().SystemOverlayLeftInset(), 0, CoreAppView().TitleBar().SystemOverlayRightInset(), 0 };
		}

		IAsyncOperation<bool> TryCloseAsync() const
		{
			return AppView().TryConsolidateAsync();
		}
	};

	struct ViewServiceDesktop : ViewServiceBase<ViewServiceDesktop>
	{
	public:
		ViewServiceDesktop(WindowHandle windowHandle) : ViewServiceBase<ViewServiceDesktop>(windowHandle) {}

		hstring Title() const
		{
			auto titleSize = static_cast<uint32>(user32.GetProcAddress<GetWindowTextLengthW>("GetWindowTextLengthW")(m_Handle) + 1);
			impl::hstring_builder stringBuilder{ titleSize };
			check_bool(user32.GetProcAddress<GetWindowTextW>("GetWindowTextW")(m_Handle, stringBuilder.data(), titleSize));
			return stringBuilder.to_hstring();
		}

		void Title(hstring const& value)
		{
			check_bool(user32.GetProcAddress<SetWindowTextW>("SetWindowTextW")(m_Handle, value.data()));
		}

		bool IsFullScreen() const
		{
			auto style = user32.GetProcAddress<GetWindowLongW>("GetWindowLongW")(m_Handle, GWL_STYLE);
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
			check_bool(user32.GetProcAddress<GetWindowRect>("GetWindowRect")(m_Handle, &result));
			float width = static_cast<float>(result.right - result.left);
			float height = static_cast<float>(result.bottom - result.top);
			return { static_cast<float>(result.left), static_cast<float>(result.top), width, height };
		}

		double TitleBarHeight() const
		{
			auto const dpi = user32.GetProcAddress<GetDpiForWindow>("GetDpiForWindow")(m_Handle);
			auto const func = user32.GetProcAddress<GetSystemMetricsForDpi>("GetSystemMetricsForDpi");
			return func(SM_CYFRAME, dpi) + func(SM_CYCAPTION, dpi) + func(SM_CXPADDEDBORDER, dpi);
		}

		Thickness TitleBarInset() const
		{
			return {};
		}

		Visibility TitleBarVisibility() const
		{
			throw hresult_not_implemented();
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
			co_return user32.GetProcAddress<DestroyWindow>("DestroyWindow")(m_Handle) != false;
		}
	private:
		DynamicLibrary user32{ L"User32.dll" };
	};

	namespace factory_implementation
	{
		UserInteractionMode ViewService::InteractionMode()
		{
			if (CoreApplication::Views().Size()) return UIViewSettings::GetForCurrentView().UserInteractionMode();
			else
			{
				uint32 result = 0;
				uint32 size = sizeof(result);
				DynamicLibrary advapi32{ L"Advapi32.dll" };
				check_win32(advapi32.GetProcAddress<RegGetValueW>("RegGetValueW")(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", L"TabletMode",
					RRF_RT_REG_DWORD, nullptr, &result, reinterpret_cast<DWORD*>(&size)));
				return static_cast<UserInteractionMode>(result);
			}
		}

		OneToolkit::UI::ViewService ViewService::GetForCurrentView()
		{
			return make<ViewServiceUniversal>();
		}

		OneToolkit::UI::ViewService ViewService::GetFromWindowHandle(WindowHandle windowHandle)
		{	
			if (CoreApplication::Views().Size()) throw hresult_error(COR_E_PLATFORMNOTSUPPORTED, L"This method is for desktop apps only. In a universal app, call ViewService.GetForCurrentView on an UI thread associated with a CoreWindow instance.");
			return make<ViewServiceDesktop>(windowHandle);
		}

		WindowHandle ViewService::GetHandleFromCoreWindow(CoreWindow const& coreWindow)
		{
			WindowHandle result;
			check_hresult(coreWindow.as<ICoreWindowInterop>()->get_WindowHandle(reinterpret_cast<HWND*>(&result.Value)));
			return result;
		}
	}
}