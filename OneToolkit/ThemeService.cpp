#include "OneToolkit.h"
#include "ThemeService.h"
#include "UI.ThemeService.g.cpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::ViewManagement;

namespace winrt::OneToolkit::UI::implementation
{
	IObservableVector<ICustomTheme> customThemes;

	ElementTheme ThemeService::AppThemeOverride() noexcept
	{
		return m_AppThemeOverride;
	}

	void ThemeService::AppThemeOverride(ElementTheme value)
	{
		if (m_AppThemeOverride != value)
		{
			auto newAppTheme = ElementToApplicationTheme(value);
			auto currentAppTheme = ElementToApplicationTheme(m_AppThemeOverride);
			m_AppThemeOverride = value;
			if (currentAppTheme != newAppTheme)
			{
				SetControlsThemeAsync(value).Completed([newAppTheme](IAsyncAction const&, AsyncStatus)
					{
						m_AppThemeChanged(newAppTheme);
					});
			}
		}
	}

	IObservableVector<ICustomTheme> ThemeService::CustomThemes() noexcept
	{
		return customThemes;
	}

	ApplicationTheme ThemeService::SystemAppsTheme()
	{
		return m_UserInterfaceSettings.GetColorValue(UIColorType::Background) == Colors::Black() ? ApplicationTheme::Dark : ApplicationTheme::Light;
	}

	ApplicationTheme ThemeService::SystemShellTheme()
	{
		throw hresult_not_implemented();
	}

	event_token ThemeService::AppThemeChanged(AppThemeChangedEventHandler const& handler)
	{
		if (handler)
		{
			AttachToColorValuesChanged();
			return m_AppThemeChanged.add(handler);
		}
	}

	void ThemeService::AppThemeChanged(event_token token) noexcept
	{
		m_AppThemeChanged.remove(token);
		if (!m_AppThemeChanged) DetachFromColorValuesChanged();
	}

	ApplicationTheme ThemeService::ElementToApplicationTheme(ElementTheme elementTheme)
	{
		if (elementTheme == ElementTheme::Default) return SystemAppsTheme();
		else return static_cast<ApplicationTheme>(static_cast<int>(elementTheme) - 1);
	}

	void ThemeService::AttachToColorValuesChanged()
	{
		m_ColorValuesChangedToken = m_UserInterfaceSettings.ColorValuesChanged([](UISettings const&, IInspectable const&)
			{
				auto currentSystemTheme = ThemeService::SystemAppsTheme();
				if (m_PreviousSystemTheme != currentSystemTheme)
				{
					m_PreviousSystemTheme = currentSystemTheme;
					if (ThemeService::AppThemeOverride() == ElementTheme::Default) AppThemeChanged(currentSystemTheme);
				}
			});
	}

	void ThemeService::DetachFromColorValuesChanged()
	{
		m_UserInterfaceSettings.ColorValuesChanged(m_ColorValuesChangedToken);
	}

	IAsyncAction ThemeService::SetControlsThemeAsync(ElementTheme elementTheme)
	{
		// Set the theme for XAML UI controls.
		if (Application::Current())
		{
			for (auto const& view : CoreApplication::Views())
			{
				co_await view.DispatcherQueue();
				if (auto window = Window::Current())
				{
					if (auto contentElement = window.Content().try_as<FrameworkElement>()) contentElement.RequestedTheme(elementTheme);
				}
			}
		}
	}
}