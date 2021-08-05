#include "pch.h"
#include "ViewService.h"

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

	using DestroyWindow = int(__stdcall*)(HWND);

	using GetWindowRect = int(__stdcall*)(HWND, LPRECT);

	__interface __declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F")) ICoreWindowInterop : ::IUnknown
	{
		int __stdcall get_WindowHandle(HWND* hwnd) noexcept;
		int __stdcall put_MessageHandled(bool value) noexcept;
	};

	namespace implementation
	{
		template <typename Derived>
		struct ViewServiceBase : ViewServiceT<Derived, non_agile>
		{
		public:
			bool IsDialogShown()
			{
				return unbox_value_or(CoreApplication::Properties().TryLookup(GenerateStoreKey(L"IsDialogShown")), false);
			}

			void IsDialogShown(bool value)
			{
				if (IsDialogShown() != value)
				{
					static slim_mutex dialogShownLock;
					const slim_lock_guard lockGuard{ dialogShownLock };
					CoreApplication::Properties().Insert(GenerateStoreKey(L"IsDialogShown"), box_value(value));
				}
			}
		protected:
			inline auto GenerateStoreKey(std::wstring_view propertyName)
			{
				return std::format(L"{0}-{1}", propertyName, static_cast<Derived*>(this)->Id().Value);
			}
		};

		struct ViewServiceUniversal : ViewServiceBase<ViewServiceUniversal>
		{
			DeclareAutoProperty(ApplicationView, AppView, ApplicationView::GetForCurrentView());

			DeclareAutoProperty(CoreApplicationView, CoreAppView, CoreApplication::GetCurrentView());

			bool IsFullScreen() const
			{
				return m_AppView.IsFullScreenMode();
			}

			void IsFullScreen(bool value)
			{
				if (m_AppView.IsFullScreenMode() != value)
				{
					if (value) m_AppView.TryEnterFullScreenMode();
					else m_AppView.ExitFullScreenMode();
				}
			}

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

			Rect Bounds() const
			{
				return m_CoreAppView.CoreWindow().Bounds();
			}

			IAsyncOperation<bool> CloseAsync() const
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
				if (Title() != value) check_bool(user32.GetProcAddress<SetWindowTextW>("SetWindowTextW")(m_WindowHandle, value.data()));
			}

			bool IsFullScreen()
			{
				return unbox_value_or(CoreApplication::Properties().TryLookup(GenerateStoreKey(L"IsFullScreen")), false);
			}

			void IsFullScreen(bool value)
			{
				if (IsFullScreen() != value)
				{
					if (value)
					{
					}
					else
					{

					}

					CoreApplication::Properties().Insert(GenerateStoreKey(L"IsFullScreen"), box_value(value));
				}
			}

			WindowId Id() const
			{
				return { as_value<uint64>(m_WindowHandle) };
			}

			Rect Bounds() const
			{
				RECT result{};
				check_bool(user32.GetProcAddress<GetWindowRect>("GetWindowRect")(m_WindowHandle, &result));
				float width = static_cast<float>(result.right - result.left);
				float height = static_cast<float>(result.bottom - result.top);
				return { static_cast<float>(result.left), static_cast<float>(result.top), width, height };
			}

			ApplicationView AppView() const noexcept
			{
				return nullptr;
			}

			CoreApplicationView CoreAppView() const noexcept
			{
				return nullptr;
			}

			IAsyncOperation<bool> CloseAsync() const
			{
				co_return user32.GetProcAddress<DestroyWindow>("DestroyWindow")(m_WindowHandle) != false;
			}
		private:
			HWND m_WindowHandle;
			DynamicModule user32 { L"User32.dll" };
		};
	}

	namespace factory_implementation
	{
		UserInteractionMode ViewService::InteractionMode()
		{
			if (CoreApplication::Views().Size()) return UIViewSettings::GetForCurrentView().UserInteractionMode();
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

		OneToolkit::UI::ViewService ViewService::GetForCurrentView()
		{
			return make<implementation::ViewServiceUniversal>();
		}

		OneToolkit::UI::ViewService ViewService::GetForWindowId(WindowId windowId)
		{
			if (CoreApplication::Views().Size()) throw hresult_illegal_method_call(L"GetForWindowId must be invoked from desktop apps only. UWP apps should invoke GetForCurrentView instead.");
			return make<implementation::ViewServiceDesktop>(windowId);
		}

		WindowId ViewService::GetCoreWindowId(CoreWindow const& coreWindow)
		{
			HWND windowHandle;
			check_hresult(coreWindow.as<ICoreWindowInterop>()->get_WindowHandle(&windowHandle));
			return { as_value<uint64>(windowHandle) };
		}
	}
}