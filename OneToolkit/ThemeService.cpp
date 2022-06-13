#include "UI.ThemeService.g.h"
#include "UI.AppThemeChangedEventArgs.g.h"
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.Collections.h>

import juv;
import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI;
using namespace Windows::UI::ViewManagement;

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		struct AppThemeChangedEventArgs : AppThemeChangedEventArgsT<AppThemeChangedEventArgs>
		{
			AppThemeChangedEventArgs(AbsoluteTheme oldTheme, AbsoluteTheme newTheme) : OldTheme(oldTheme), NewTheme(newTheme) {}
			auto_property<AbsoluteTheme> const OldTheme;
			auto_property<AbsoluteTheme> const NewTheme;
		};

		struct ThemeService : ThemeServiceT<ThemeService>
		{
		public:
			ThemeService()
			{
				m_ColorValuesChangedToken = m_UserInterfaceSettings.ColorValuesChanged({ this, &ThemeService::ColorValuesChangedHandler });
			}

			~ThemeService()
			{
				m_UserInterfaceSettings.ColorValuesChanged(m_ColorValuesChangedToken);
			}

			void ColorValuesChangedHandler(UISettings const&, IInspectable const&)
			{
				auto const currentSystemTheme = SystemAppsTheme();
				auto const previousSystemAppsTheme = PreviousSystemAppsTheme();
				if (previousSystemAppsTheme != currentSystemTheme)
				{
					PreviousSystemAppsTheme(currentSystemTheme);
					if (ThemeService::AppThemeOverride() == RelativeTheme::Default) m_AppThemeChanged(make<AppThemeChangedEventArgs>(previousSystemAppsTheme, currentSystemTheme));
					else SetControlsThemeAsync(AppThemeOverride());
				}
			}

			static RelativeTheme AppThemeOverride() noexcept
			{
				return Globals().m_AppThemeOverride;
			}

			static void AppThemeOverride(RelativeTheme value)
			{
				auto const appThemeOverride = Globals().m_AppThemeOverride.load();
				if (appThemeOverride != value)
				{
					auto newAppTheme = ThemeConverter::ToAbsoluteTheme(value, SystemAppsTheme());
					auto currentAppTheme = ThemeConverter::ToAbsoluteTheme(appThemeOverride, SystemAppsTheme());
					Globals().m_AppThemeOverride = value;
					if (currentAppTheme != newAppTheme)
					{
						SetControlsThemeAsync(value).Completed([currentAppTheme, newAppTheme](IAsyncAction const&, AsyncStatus)
							{
								Globals().m_AppThemeChanged(make<AppThemeChangedEventArgs>(currentAppTheme, newAppTheme));
							});
					}
				}
			}

			static AbsoluteTheme SystemAppsTheme()
			{
				return Globals().m_UserInterfaceSettings.GetColorValue(UIColorType::Background) == Colors::Black() ? AbsoluteTheme::Dark : AbsoluteTheme::Light;
			}

			static AbsoluteTheme SystemShellTheme()
			{
				throw hresult_not_implemented();
			}

			static event_token AppThemeChanged(AppThemeChangedEventHandler const& handler)
			{
				return Globals().m_AppThemeChanged.add(handler);
			}

			static void AppThemeChanged(event_token token) noexcept
			{
				Globals().m_AppThemeChanged.remove(token);
			}

			static void Initialize()
			{
				if (!s_Globals) s_Globals = make_self<ThemeService>();
			}

			static void Uninitialize()
			{
				s_Globals = nullptr;
			}
		private:
			inline static com_ptr<ThemeService> s_Globals;

			event_token m_ColorValuesChangedToken;

			UISettings const m_UserInterfaceSettings;

			std::atomic<std::optional<AbsoluteTheme>> m_PreviousSystemAppsTheme;

			std::atomic<RelativeTheme> m_AppThemeOverride = RelativeTheme::Default;

			event<AppThemeChangedEventHandler> m_AppThemeChanged;

			AbsoluteTheme PreviousSystemAppsTheme()
			{
				auto const value = m_PreviousSystemAppsTheme.load();
				if (!value)
				{
					auto const systemAppsTheme = SystemAppsTheme();
					m_PreviousSystemAppsTheme = systemAppsTheme;
					return systemAppsTheme;
				}

				return *value;
			}

			static ThemeService& Globals()
			{
				if (!s_Globals) throw hresult_invalid_state(L"ThemeService hasn't been initialized yet.");
				return *s_Globals;
			}

			static void PreviousSystemAppsTheme(AbsoluteTheme newValue) noexcept
			{
				Globals().m_PreviousSystemAppsTheme = newValue;
			}

			static IAsyncAction SetControlsThemeAsync(RelativeTheme elementTheme)
			{
				// Set the theme for XAML UI controls.
				if (Windows::UI::Xaml::Application::Current())
				{
					for (auto const& view : CoreApplication::Views())
					{
						co_await view.DispatcherQueue();
						if (auto window = Windows::UI::Xaml::Window::Current())
						{
							if (auto contentElement = window.Content().try_as<Windows::UI::Xaml::FrameworkElement>()) contentElement.RequestedTheme(static_cast<Windows::UI::Xaml::ElementTheme>(elementTheme));
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
#include "UI.AppThemeChangedEventArgs.g.cpp"