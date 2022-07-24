#include "UI.Theming.ThemeService.g.h"
#include <Windows.h>
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

namespace winrt::OneToolkit::UI::Theming
{
	namespace implementation
	{
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
					if (ThemeService::AppThemeOverride() == RelativeTheme::Default) {} //m_AppThemeChanged(make<AppThemeChangedEventArgs>(previousSystemAppsTheme, currentSystemTheme));
					else SetControlsThemeAsync(AppThemeOverride());
				}
			}

			RelativeTheme AppThemeOverride() noexcept
			{
				return m_AppThemeOverride;
			}

			void AppThemeOverride(RelativeTheme value)
			{
				auto const appThemeOverride = m_AppThemeOverride.load();
				if (appThemeOverride != value)
				{
					auto newAppTheme = ThemeConverter::ToAbsoluteTheme(value, SystemAppsTheme());
					auto currentAppTheme = ThemeConverter::ToAbsoluteTheme(appThemeOverride, SystemAppsTheme());
					m_AppThemeOverride = value;
					if (currentAppTheme != newAppTheme)
					{
						SetControlsThemeAsync(value).Completed([=](IAsyncAction const&, AsyncStatus)
							{
								//m_AppThemeChanged(make<AppThemeChangedEventArgs>(currentAppTheme, newAppTheme));
							});
					}
				}
			}

			AbsoluteTheme SystemAppsTheme()
			{
				return m_UserInterfaceSettings.GetColorValue(UIColorType::Background) == Colors::Black() ? AbsoluteTheme::Dark : AbsoluteTheme::Light;
			}

			AbsoluteTheme SystemShellTheme()
			{
				uint32 result;
				uint32 resultSize;
				check_win32(RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"SystemUsesLightTheme", RRF_RT_REG_DWORD, nullptr, &result, reinterpret_cast<DWORD*>(&resultSize)));
				return static_cast<AbsoluteTheme>(!result);
			}

			static OneToolkit::UI::Theming::ThemeService Current()
			{
				static auto const instance = make<ThemeService>();
				return instance;
			}
		private:
			event_token m_ColorValuesChangedToken;

			UISettings const m_UserInterfaceSettings;

			std::atomic<std::optional<AbsoluteTheme>> m_PreviousSystemAppsTheme;

			std::atomic<RelativeTheme> m_AppThemeOverride = RelativeTheme::Default;

			//event<AppThemeChangedEventHandler> m_AppThemeChanged;

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

			void PreviousSystemAppsTheme(AbsoluteTheme newValue) noexcept
			{
				m_PreviousSystemAppsTheme = newValue;
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

#include "UI.Theming.ThemeService.g.cpp"
#include "UI.Theming.AppThemeChangedEventArgs.g.cpp"