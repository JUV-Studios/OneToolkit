#include "UI.ThemeService.g.h"
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.Collections.h>

import OneToolkit;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::Lifecycle;

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		struct AppThemeChangedAsyncEventHandler
		{
		public:
			AppThemeChangedAsyncEventHandler() = default;

			AppThemeChangedAsyncEventHandler(AppThemeChangedEventHandler const& handler, apartment_context threadContext = {}) : m_Delegate(handler), m_ThreadContext(threadContext) {}

			operator void* () const noexcept
			{
				return get_abi(m_Delegate);
			}

			fire_and_forget operator()(Windows::UI::Xaml::ApplicationTheme newTheme) const
			{
				co_await m_ThreadContext;
				m_Delegate(newTheme);
			}
		private:
			apartment_context m_ThreadContext;
			mutable AppThemeChangedEventHandler m_Delegate;
		};

		struct ThemeService : ThemeServiceT<ThemeService>
		{
		public:
			static Windows::UI::Xaml::ElementTheme AppThemeOverride() noexcept
			{
				return unbox_value_or(CoreApplication::Properties().TryLookup(L"OneToolkit.UI.ThemeService.AppThemeOverride"), Windows::UI::Xaml::ElementTheme::Default);
			}

			static void AppThemeOverride(Windows::UI::Xaml::ElementTheme value)
			{
				auto const appThemeOverride = AppThemeOverride();
				if (appThemeOverride != value)
				{
					auto newAppTheme = ElementToApplicationTheme(value);
					auto currentAppTheme = ElementToApplicationTheme(appThemeOverride);
					CoreApplication::Properties().Insert(L"OneToolkit.UI.ThemeService.AppThemeOverride", box_value(value));
					if (currentAppTheme != newAppTheme)
					{
						SetControlsThemeAsync(value).Completed([newAppTheme](IAsyncAction const&, AsyncStatus)
							{
								m_AppThemeChanged(newAppTheme);
							});
					}
				}
			}

			static IObservableVector<ICustomTheme> CustomThemes() noexcept
			{
				static slim_mutex mutex;
				slim_lock_guard const mutexLock{ mutex };
				if (!m_CutomThemes) m_CutomThemes = multi_threaded_observable_vector<ICustomTheme>();
				return m_CutomThemes;
			}

			static Windows::UI::Xaml::ApplicationTheme SystemAppsTheme()
			{
				return m_UserInterfaceSettings.GetColorValue(UIColorType::Background) == Colors::Black() ? Windows::UI::Xaml::ApplicationTheme::Dark : Windows::UI::Xaml::ApplicationTheme::Light;
			}

			static Windows::UI::Xaml::ApplicationTheme SystemShellTheme()
			{
				throw hresult_not_implemented();
			}

			static event_token AppThemeChanged(AppThemeChangedEventHandler const& handler)
			{
				if (!m_AppThemeChanged)
				{
					// Attach to forward system theme changes to the AppThemeChanged event.
					m_ColorValuesChangedToken = m_UserInterfaceSettings.ColorValuesChanged([](UISettings const&, IInspectable const&)
						{
							auto currentSystemTheme = ThemeService::SystemAppsTheme();
							if (PreviousSystemAppsTheme() != currentSystemTheme)
							{
								PreviousSystemAppsTheme(currentSystemTheme);
								if (ThemeService::AppThemeOverride() == Windows::UI::Xaml::ElementTheme::Default) m_AppThemeChanged(currentSystemTheme);
								else SetControlsThemeAsync(AppThemeOverride());
							}
						});

					handler(ElementToApplicationTheme(AppThemeOverride()));
				}

				return m_AppThemeChanged.add(handler);
			}

			static void AppThemeChanged(event_token token) noexcept
			{
				m_AppThemeChanged.remove(token);
				if (!m_AppThemeChanged)
				{
					// Detach from handling system theme changes as now there's no listener for the AppThemeChanged event.
					m_UserInterfaceSettings.ColorValuesChanged(m_ColorValuesChangedToken);
				}
			}

			static Windows::UI::Xaml::ApplicationTheme ElementToApplicationTheme(Windows::UI::Xaml::ElementTheme elementTheme)
			{
				if (elementTheme == Windows::UI::Xaml::ElementTheme::Default) return SystemAppsTheme();
				else return static_cast<Windows::UI::Xaml::ApplicationTheme>(static_cast<int>(elementTheme) - 1);
			}
		private:
			inline static UISettings m_UserInterfaceSettings;

			inline static event_token m_ColorValuesChangedToken;

			inline static IObservableVector<ICustomTheme> m_CutomThemes;

			inline static event<AppThemeChangedAsyncEventHandler> m_AppThemeChanged;

			inline static std::atomic<std::optional<Windows::UI::Xaml::ApplicationTheme>> m_PreviousSystemAppsTheme;

			static Windows::UI::Xaml::ApplicationTheme PreviousSystemAppsTheme()
			{
				if (!m_PreviousSystemAppsTheme.load()) m_PreviousSystemAppsTheme = SystemAppsTheme();
				return *m_PreviousSystemAppsTheme.load();
			}

			static void PreviousSystemAppsTheme(Windows::UI::Xaml::ApplicationTheme const newValue) noexcept
			{
				m_PreviousSystemAppsTheme = newValue;
			}

			static IAsyncAction SetControlsThemeAsync(Windows::UI::Xaml::ElementTheme elementTheme)
			{
				// Set the theme for XAML UI controls.
				if (Windows::UI::Xaml::Application::Current())
				{
					for (auto const& view : CoreApplication::Views())
					{
						co_await view.DispatcherQueue();
						if (auto window = Windows::UI::Xaml::Window::Current())
						{
							if (auto contentElement = window.Content().try_as<Windows::UI::Xaml::FrameworkElement>()) contentElement.RequestedTheme(elementTheme);
						}
					}
				}
				else if (Microsoft::UI::Xaml::Application::Current())
				{
				}
			}
		};
	}

	namespace factory_implementation
	{
		struct ThemeService : ThemeServiceT<ThemeService, implementation::ThemeService>
		{
		};
	}
}

#include "UI.ThemeService.g.cpp"