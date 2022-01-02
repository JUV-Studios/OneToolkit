#include "UI.ThemeService.g.h"

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
			ThemeService() = delete;
			static Windows::UI::Xaml::ElementTheme AppThemeOverride() noexcept;
			static void AppThemeOverride(Windows::UI::Xaml::ElementTheme value);
			static Windows::Foundation::Collections::IObservableVector<ICustomTheme> CustomThemes() noexcept;
			static Windows::UI::Xaml::ApplicationTheme SystemAppsTheme();
			static Windows::UI::Xaml::ApplicationTheme SystemShellTheme();
			static event_token AppThemeChanged(AppThemeChangedEventHandler const& handler);
			static void AppThemeChanged(event_token token) noexcept;
			static Windows::UI::Xaml::ApplicationTheme ElementToApplicationTheme(Windows::UI::Xaml::ElementTheme elementTheme);
		private:
			static event_token m_ColorValuesChangedToken;
			static event<AppThemeChangedAsyncEventHandler> m_AppThemeChanged;
			static Windows::UI::ViewManagement::UISettings m_UserInterfaceSettings;
			inline static std::optional<Windows::UI::Xaml::ApplicationTheme> m_PreviousSystemTheme = SystemAppsTheme();
			inline static Windows::UI::Xaml::ElementTheme m_AppThemeOverride = Windows::UI::Xaml::ElementTheme::Default;
			static void AttachToColorValuesChanged();
			static void DetachFromColorValuesChanged();
			static Windows::Foundation::IAsyncAction SetControlsThemeAsync(Windows::UI::Xaml::ElementTheme elementTheme);
		};
	}

	namespace factory_implementation
	{
		struct ThemeService : ThemeServiceT<ThemeService, implementation::ThemeService>
		{
		};
	}
}