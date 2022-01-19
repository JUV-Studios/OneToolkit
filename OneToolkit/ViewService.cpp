#include "UI.ViewService.g.h"
#include <Unknwn.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.WindowManagement.Preview.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Foundation.Collections.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::ViewManagement;
using namespace Microsoft::UI::Windowing;
using namespace OneToolkit::System;
using namespace OneToolkit::Lifecycle;
using namespace OneToolkit::Data::Mvvm;

namespace winrt::OneToolkit::UI
{
	std::map<uint64, weak_ref<OneToolkit::UI::ViewService>> viewServiceCache;

	namespace implementation
	{
		template <typename Derived>
		struct ViewServiceBase : implementation::ViewServiceT<Derived, non_agile>, ObservableBase<Derived>, Equatable<Derived>
		{
		public:
			void ToggleFullScreen() const
			{
				static_cast<Derived const* const>(this)->IsInFullScreen(!static_cast<Derived const* const>(this)->IsInFullScreen());
			}

			void ToggleCompactOverlay() const
			{
				static_cast<Derived const* const>(this)->IsInCompactOverlay(!static_cast<Derived const* const>(this)->IsInCompactOverlay());
			}

			bool operator==(IInspectable const& another) const noexcept
			{
				if (auto viewService = another.try_as<OneToolkit::UI::ViewService>()) return viewService.Id() == static_cast<Derived const* const>(this)->Id();
				else return false;
			}

			~ViewServiceBase()
			{
				viewServiceCache.erase(static_cast<Derived const* const>(this)->Id().Value);
			}
		protected:
			void Activate()
			{
				viewServiceCache[static_cast<Derived const* const>(this)->Id().Value] = *static_cast<Derived* const>(this);
			}
		};

		struct ViewServiceUniversal : ViewServiceBase<ViewServiceUniversal>
		{
			ViewServiceUniversal()
			{
				CoreAppView().CoreWindow().VisibilityChanged([weakThis = get_weak()](CoreWindow, VisibilityChangedEventArgs)
				{
					if (auto strongThis = weakThis.get())
					{
						strongThis->Raise(L"Visibility");
					}
				});

				CoreAppView().CoreWindow().SizeChanged([weakThis = get_weak()](CoreWindow, WindowSizeChangedEventArgs)
				{
					if (auto strongThis = weakThis.get())
					{
						strongThis->Raise(L"Bounds");
						strongThis->Raise(L"IsInFullScreen");
						strongThis->Raise(L"IsInCompactOverlay");
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

				Activate();
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
				if (AppView().Title() != value)
				{
					AppView().Title(value);
					Raise(L"Title");
					Raise(L"ActualTitle");
				}
			}

			hstring ActualTitle() const
			{
				auto actualTitle = AppView().Title();
				actualTitle = actualTitle.empty() ? AppInformation::Current().DisplayName() : std::format(L"{} - {}", actualTitle, AppInformation::Current().DisplayName()).data();
				return actualTitle;
			}

			bool IsInFullScreen() const
			{
				return AppView().IsFullScreenMode();
			}

			void IsInFullScreen(bool value) const
			{
				if (AppView().IsFullScreenMode() != value)
				{
					if (value) AppView().TryEnterFullScreenMode();
					else AppView().ExitFullScreenMode();
				}
			}

			bool IsInCompactOverlay() const
			{
				return AppView().ViewMode() == ApplicationViewMode::CompactOverlay;
			}

			void IsInCompactOverlay(bool value) const
			{
				if (IsInCompactOverlay() != value)
				{
					if (value) AppView().TryEnterViewModeAsync(ApplicationViewMode::CompactOverlay);
					else AppView().TryEnterViewModeAsync(ApplicationViewMode::Default);
				}
			}

			WindowId Id() const noexcept
			{
				return { static_cast<uint64>(AppView().Id()) };
			}

			Rect Bounds() const
			{
				return CoreAppView().CoreWindow().Bounds();
			}

			::Visibility Visibility() const
			{
				return CoreAppView().CoreWindow().Visible() ? Visibility::Visible : Visibility::Collapsed;
			}

			double TitleBarHeight() const
			{
				return CoreAppView().TitleBar().Height();
			}

			::Visibility TitleBarVisibility() const
			{
				return CoreAppView().TitleBar().IsVisible() ? Visibility::Visible : Visibility::Collapsed;
			}

			Thickness TitleBarInset() const
			{
				return { CoreAppView().TitleBar().SystemOverlayLeftInset(), 0, CoreAppView().TitleBar().SystemOverlayRightInset(), 0 };
			}

			IInspectable AppWindowReference() const noexcept
			{
				return nullptr;
			}

			bool TrySetMinimumSize(Size minSize) const
			{
				AppView().SetPreferredMinSize(minSize);
				return true;
			}

			IAsyncOperation<bool> TryCloseAsync() const
			{
				return AppView().TryConsolidateAsync();
			}

			IAsyncOperation<bool> TryMinimizeAsync() const
			{
				if (CoreApplication::Views().Size() == 1)
				{
					co_await(co_await AppDiagnosticInfo::RequestInfoForAppAsync()).GetAt(0).GetResourceGroups().GetAt(0).StartSuspendAsync();
					co_return true;
				}

				co_return false;
			}
		};

		struct ViewServiceUniversalPreview : ViewServiceBase<ViewServiceUniversalPreview>
		{
			ViewServiceUniversalPreview(Windows::UI::WindowManagement::AppWindow const& appWindow) : AppWindowReference(appWindow)
			{
				AppWindowReference().Changed([weakThis = get_weak()](Windows::UI::WindowManagement::AppWindow const&, Windows::UI::WindowManagement::AppWindowChangedEventArgs const& args)
				{
					if (auto strongThis = weakThis.get())
					{
						if (args.DidWindowPresentationChange())
						{
							strongThis->Raise(L"IsInFullScreen");
							strongThis->Raise(L"IsInCompactOverlay");
						}
					}
				});

				Activate();
			}

			auto_property<Windows::UI::WindowManagement::AppWindow> const AppWindowReference;

			hstring Title() const
			{
				return AppWindowReference().Title();
			}

			void Title(hstring const& value)
			{
				if (AppWindowReference().Title() != value)
				{
					AppWindowReference().Title(value);
					Raise(L"Title");
					Raise(L"ActualTitle");
				}
			}

			hstring ActualTitle() const
			{
				return AppWindowReference().Title();
			}

			bool IsInFullScreen() const
			{
				return AppWindowReference().Presenter().GetConfiguration().Kind() == Windows::UI::WindowManagement::AppWindowPresentationKind::FullScreen;
			}

			void IsInFullScreen(bool value) const
			{
				if (IsInFullScreen() != value) AppWindowReference().Presenter().RequestPresentation(value ? Windows::UI::WindowManagement::AppWindowPresentationKind::FullScreen : Windows::UI::WindowManagement::AppWindowPresentationKind::Default);
			}

			bool IsInCompactOverlay() const
			{
				return AppWindowReference().Presenter().GetConfiguration().Kind() == Windows::UI::WindowManagement::AppWindowPresentationKind::CompactOverlay;
			}

			void IsInCompactOverlay(bool value) const
			{
				if (IsInCompactOverlay() != value) AppWindowReference().Presenter().RequestPresentation(value ? Windows::UI::WindowManagement::AppWindowPresentationKind::CompactOverlay : Windows::UI::WindowManagement::AppWindowPresentationKind::Default);
			}

			WindowId Id() const noexcept
			{
				// UWP AppWindow doesn't provide a way to obtain the window handle so we'll return the address of the owning object instead.
				return { as_value<uint64>(::get_unknown(AppWindowReference())) };
			}

			Rect Bounds() const
			{
				throw hresult_not_implemented();
			}

			::Visibility Visibility() const
			{
				return AppWindowReference().IsVisible() ? Visibility::Visible : Visibility::Collapsed;
			}

			double TitleBarHeight() const
			{
				return AppWindowReference().TitleBar().GetTitleBarOcclusions().GetAt(0).OccludingRect().Height;
			}

			Thickness TitleBarInset() const
			{
				throw hresult_not_implemented();
			}

			::Visibility TitleBarVisibility() const
			{
				return AppWindowReference().TitleBar().IsVisible() ? Visibility::Visible : Visibility::Collapsed;
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

			bool TrySetMinimumSize(Size minSize) const
			{
				if (ApiInformation::IsTypePresent(name_of<Windows::UI::WindowManagement::Preview::WindowManagementPreview>()))
				{
					Windows::UI::WindowManagement::Preview::WindowManagementPreview::SetPreferredMinSize(AppWindowReference(), minSize);
					return true;
				}
				else
				{
					return false;
				}
			}

			IAsyncOperation<bool> TryCloseAsync() const
			{
				co_await AppWindowReference().CloseAsync();
				co_return true;
			}

			IAsyncOperation<bool> TryMinimizeAsync() const
			{
				co_return false;
			}
		};

		struct ViewServiceDesktop : ViewServiceBase<ViewServiceDesktop>
		{
		public:
			ViewServiceDesktop(AppWindow const& appWindow) : AppWindowReference(appWindow)
			{
				// Due to a bug in WindowsAppSDK, the AppWindow.Changed event shouldn't be registered from a WinUI app. See https://github.com/microsoft/microsoft-ui-xaml/issues/6466 for more information.
				if (!AppInformation::IsXamlApplication())
				{
					AppWindowReference().Changed([weakThis = get_weak()](AppWindow const&, AppWindowChangedEventArgs const& args)
					{
						if (auto strongThis = weakThis.get())
						{
							if (args.DidPresenterChange())
							{
								strongThis->Raise(L"IsInFullScreen");
								strongThis->Raise(L"IsInCompactOverlay");
								strongThis->Raise(L"TitleBarInset");
								strongThis->Raise(L"TitleBarHeight");
								strongThis->m_PreviousPresenterKind = strongThis->AppWindowReference().Presenter().Kind();
							}

							if (args.DidSizeChange() || args.DidPositionChange()) strongThis->Raise(L"Bounds");
						}
					});
				}

				Activate();
			}

			auto_property<AppWindow> const AppWindowReference;

			hstring Title() const
			{
				return AppWindowReference().Title();
			}

			void Title(hstring const& value)
			{
				if (AppWindowReference().Title() != value)
				{
					AppWindowReference().Title(value);
					Raise(L"Title");
					Raise(L"ActualTitle");
				}
			}

			hstring ActualTitle() const
			{
				return AppWindowReference().Title();
			}

			bool IsInFullScreen() const
			{
				return AppWindowReference().Presenter().Kind() == AppWindowPresenterKind::FullScreen;
			}

			void IsInFullScreen(bool value) const
			{
				if (IsInFullScreen() != value) AppWindowReference().SetPresenter(value ? AppWindowPresenterKind::FullScreen : m_PreviousPresenterKind);
			}

			bool IsInCompactOverlay() const
			{
				return AppWindowReference().Presenter().Kind() == AppWindowPresenterKind::CompactOverlay;
			}

			void IsInCompactOverlay(bool value) const
			{
				if (IsInFullScreen() != value) AppWindowReference().SetPresenter(value ? AppWindowPresenterKind::FullScreen : m_PreviousPresenterKind);
			}

			WindowId Id() const noexcept
			{
				return { AppWindowReference().Id().Value };
			}

			Rect Bounds() const
			{
				auto const size = AppWindowReference().Size();
				auto const position = AppWindowReference().Position();
				return { static_cast<float>(position.X), static_cast<float>(position.Y), static_cast<float>(size.Width), static_cast<float>(size.Height) };
			}

			::Visibility Visibility() const
			{
				return AppWindowReference().IsVisible() ? Visibility::Visible : Visibility::Collapsed;
			}

			double TitleBarHeight() const
			{
				return static_cast<double>(AppWindowReference().TitleBar().Height());
			}

			Thickness TitleBarInset() const
			{
				return { static_cast<double>(AppWindowReference().TitleBar().LeftInset()), 0, static_cast<double>(AppWindowReference().TitleBar().RightInset()), 0 };
			}

			::Visibility TitleBarVisibility() const
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

			bool TrySetMinimumSize(Size minSize) const
			{
				return false;
			}

			IAsyncOperation<bool> TryCloseAsync() const
			{
				AppWindowReference().Destroy();
				co_return true;
			}

			IAsyncOperation<bool> TryMinimizeAsync() const
			{
				if (auto overlappedPresenter = AppWindowReference().Presenter().try_as<OverlappedPresenter>())
				{
					overlappedPresenter.Minimize();
					co_return true;
				}

				co_return false;
			}
		private:
			AppWindowPresenterKind m_PreviousPresenterKind = AppWindowPresenterKind::Overlapped;
		};
	}

	namespace factory_implementation
	{
		using WindowEnumProcedure = int(__stdcall*)(WindowId, void*);

		using FindWindowW = WindowId(__stdcall*)(wchar_t const*, wchar_t const*);

		using EnumThreadWindows = bool(__stdcall*)(uint32, WindowEnumProcedure, void*);

		using RegGetValueW = int(__stdcall*)(void*, wchar_t const*, wchar_t const*, uint32, uint32*, void*, uint32*);

		__interface __declspec(uuid("EECDBF0E-BAE9-4CB6-A68E-9598E1CB57BB")) IWindowNative : ::IUnknown
		{
			int __stdcall get_WindowHandle(WindowId * windowHandle);
		};

		struct ViewService : ViewServiceT<ViewService, ViewService>
		{
			static UserInteractionMode InteractionMode()
			{
				if (CoreApplication::Views().Size()) return UIViewSettings::GetForCurrentView().UserInteractionMode();
				else
				{
					uint32 result = 0;
					uint32 size = sizeof(result);
					DynamicLibrary advapi32{ L"Advapi32.dll" };
					check_win32(advapi32.GetProcAddress<RegGetValueW>("RegGetValueW")(HKEY_CURRENT_USER, L"Software\\MonitorInfocrosoft\\Windows\\CurrentVersion\\ImmersiveShell", L"TabletMode",
						RRF_RT_REG_DWORD, nullptr, &result, &size));
					return static_cast<UserInteractionMode>(result);
				}
			}

			static OneToolkit::UI::ViewService GetForCurrentView()
			{
				return make<implementation::ViewServiceUniversal>();
			}

			static OneToolkit::UI::ViewService GetFromAppWindow(IInspectable const& appWindow)
			{
				if (auto appWindowReunion = appWindow.try_as<AppWindow>()) return make<implementation::ViewServiceDesktop>(appWindowReunion);
				else if (auto appWindowPreview = appWindow.try_as<Windows::UI::WindowManagement::AppWindow>()) return make<implementation::ViewServiceUniversalPreview>(appWindowPreview);
				else return nullptr;
			}

			static OneToolkit::UI::ViewService GetFromWindowId(Windows::UI::WindowId windowId)
			{
				if (CoreApplication::Views().Size()) throw hresult_platform_not_supported(L"Cannot get a ViewService instance from window id under UWP.");
				if (windowId.Value)
				{
					if (auto iterator = viewServiceCache.find(windowId.Value); iterator != viewServiceCache.end())
					{
						auto const cachedViewService = iterator->second;
						if (cachedViewService) return cachedViewService.get();
					}

					return make<implementation::ViewServiceDesktop>(AppWindow::GetFromWindowId(static_cast<Microsoft::UI::WindowId>(windowId.Value)));
				}

				return nullptr;
			}

			static IAsyncOperation<OneToolkit::UI::ViewService> GetFromXamlWindowAsync(IInspectable const& xamlWindow)
			{
				OneToolkit::UI::ViewService result = nullptr;
				if (auto nativeWindow = xamlWindow.try_as<IWindowNative>())
				{
					WindowId windowId;
					check_hresult(nativeWindow->get_WindowHandle(&windowId));
					result = GetFromWindowId(windowId);
				}
				else
				{
					apartment_context threadContext;
					DispatcherQueue dispatcherQueue = nullptr;
					if (auto platformWindow = xamlWindow.try_as<Window>()) dispatcherQueue = platformWindow.CoreWindow().DispatcherQueue();
					else if (auto liftedWindow = xamlWindow.try_as<Microsoft::UI::Xaml::Window>()) dispatcherQueue = liftedWindow.CoreWindow().DispatcherQueue();
					else co_return nullptr;
					co_await dispatcherQueue;
					result = GetForCurrentView();
					co_await threadContext;
					co_return result;
				}

				co_return result;
			}

			static IAsyncOperation<OneToolkit::UI::ViewService> FindAsync(hstring const& windowTitle, hstring const& windowClassName = {})
			{
				OneToolkit::UI::ViewService result = nullptr;
				if (CoreApplication::Views().Size())
				{
					apartment_context threadContext;
					for (auto const& view : CoreApplication::Views())
					{
						co_await view.DispatcherQueue();
						if (ApplicationView::GetForCurrentView().Title() == windowTitle) result = GetForCurrentView();
					}

					co_await threadContext;
				}
				else
				{
					DynamicLibrary user32{ L"User32.dll" };
					result = GetFromWindowId(user32.GetProcAddress<FindWindowW>("FindWindowW")(windowClassName.empty() ? nullptr : windowClassName.data(), windowTitle.data()));
				}

				co_return result;
			}

			static IAsyncOperation<IVectorView<OneToolkit::UI::ViewService>> EnumerateAsync()
			{
				std::vector<OneToolkit::UI::ViewService> result;
				if (CoreApplication::Views().Size())
				{
					apartment_context threadContext;
					for (auto const& view : CoreApplication::Views())
					{
						co_await view.DispatcherQueue();
						result.push_back(GetForCurrentView());
					}

					co_await threadContext;
				}
				else
				{
					DynamicLibrary user32{ L"User32.dll" };
					auto enumProcedure = [](WindowId windowId, void* param) -> int
					{
						static_cast<std::vector<OneToolkit::UI::ViewService>*>(param)->push_back(GetFromWindowId(windowId));
						return true;
					};

					for (auto threadId : co_await ThreadHelper::EnumerateAsync())
					{
						user32.GetProcAddress<EnumThreadWindows>("EnumThreadWindows")(threadId.Value, enumProcedure, &result);
					}
				}

				co_return multi_threaded_vector(std::move(result)).GetView();
			}
		};
	}

	namespace implementation
	{
		using ViewService = factory_implementation::ViewService;
	}
}

#include "UI.ViewService.g.cpp"