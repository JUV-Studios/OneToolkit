module;

#include "WindowingBase.h"
#include "UI.Windowing.ViewReferenceTitleBar.g.h"

#define color_value_accessor(name) Windows::Foundation::IReference<Windows::UI::Color> name() const { return static_cast<Derived const&>(*this).ColorValues().name(); }\
void name(Windows::Foundation::IReference<Windows::UI::Color> const& value) const { static_cast<Derived const&>(*this).ColorValues().name(value); }

export module ViewReferenceTitleBar;

using namespace juv;

export namespace winrt::OneToolkit::UI::Windowing::implementation
{
	template <typename Derived, typename... Interfaces>
	struct ViewReferenceTitleBarBase : ViewReferenceTitleBarT<Derived>
	{
		color_value_accessor(BackgroundColor);

		color_value_accessor(ForegroundColor);

		color_value_accessor(InactiveBackgroundColor);

		color_value_accessor(InactiveForegroundColor);

		color_value_accessor(ButtonBackgroundColor);

		color_value_accessor(ButtonForegroundColor);

		color_value_accessor(ButtonHoverBackgroundColor);

		color_value_accessor(ButtonHoverForegroundColor);

		color_value_accessor(ButtonInactiveBackgroundColor);

		color_value_accessor(ButtonInactiveForegroundColor);

		color_value_accessor(ButtonPressedBackgroundColor);

		color_value_accessor(ButtonPressedForegroundColor);

		void ResetToDefault() const
		{
			BackgroundColor(nullptr);
			ForegroundColor(nullptr);
			InactiveBackgroundColor(nullptr);
			InactiveForegroundColor(nullptr);
			ButtonBackgroundColor(nullptr);
			ButtonForegroundColor(nullptr);
			ButtonHoverBackgroundColor(nullptr);
			ButtonHoverForegroundColor(nullptr);
			ButtonInactiveBackgroundColor(nullptr);
			ButtonInactiveForegroundColor(nullptr);
			ButtonPressedBackgroundColor(nullptr);
			ButtonPressedForegroundColor(nullptr);
			static_cast<Derived const&>(*this).ExtendsContentIntoTitleBar(false);
		}
	};

	namespace Universal::Core
	{
		struct ViewReferenceTitleBar : ViewReferenceTitleBarBase<ViewReferenceTitleBar>
		{
		public:
			ViewReferenceTitleBar(ApplicationViewTitleBar const& appViewTitleBar, CoreApplicationViewTitleBar const& coreAppViewTitleBar) : m_AppViewTitleBar(appViewTitleBar), m_CoreAppViewTitleBar(coreAppViewTitleBar) {}

			auto ColorValues() const
			{
				return m_AppViewTitleBar;
			}

			double Height() const
			{
				return m_CoreAppViewTitleBar.Height();
			}

			bool IsVisible() const
			{
				return m_CoreAppViewTitleBar.IsVisible();
			}

			double LeftInset() const
			{
				return m_CoreAppViewTitleBar.SystemOverlayLeftInset();
			}

			double RightInset() const
			{
				return m_CoreAppViewTitleBar.SystemOverlayRightInset();
			}

			bool ExtendsContentIntoTitleBar() const
			{
				return m_CoreAppViewTitleBar.ExtendViewIntoTitleBar();
			}

			void ExtendsContentIntoTitleBar(bool value) const
			{
				m_CoreAppViewTitleBar.ExtendViewIntoTitleBar(value);
			}

			com_array<IInspectable> BackingObjects() const
			{
				return { m_AppViewTitleBar, m_CoreAppViewTitleBar };
			}
		private:
			ApplicationViewTitleBar const m_AppViewTitleBar;
			CoreApplicationViewTitleBar const m_CoreAppViewTitleBar;
		};
	}

	namespace Universal::Preview
	{
		struct ViewReferenceTitleBar : ViewReferenceTitleBarBase<ViewReferenceTitleBar>
		{
		public:
			ViewReferenceTitleBar(AppWindowTitleBar const& appWindowTitleBar) : m_AppWindowTitleBar(appWindowTitleBar) {}

			double Height() const
			{
				// TOOD return Controller().GetTitleBarOcclusions().GetAt(0).OccludingRect().Height;
				return 32;
			}

			bool IsVisible() const
			{
				return m_AppWindowTitleBar.IsVisible();
			}

			double LeftInset() const
			{
				return 0;
			}

			double RightInset() const
			{
				return 0;
			}

			auto ColorValues() const
			{
				return m_AppWindowTitleBar;
			}

			com_array<IInspectable> BackingObjects() const
			{
				return { m_AppWindowTitleBar };
			}

			bool ExtendsContentIntoTitleBar() const
			{
				return m_AppWindowTitleBar.ExtendsContentIntoTitleBar();
			}

			void ExtendsContentIntoTitleBar(bool value) const
			{
				m_AppWindowTitleBar.ExtendsContentIntoTitleBar(value);
			}
		private:
			AppWindowTitleBar const m_AppWindowTitleBar;
		};
	}

	namespace Desktop
	{
		struct ViewReferenceTitleBar : ViewReferenceTitleBarBase<ViewReferenceTitleBar>
		{
		public:
			ViewReferenceTitleBar(AppWindow const& appWindow) : m_AppWindow(appWindow) {}

			double Height() const
			{
				return static_cast<double>(m_AppWindow.TitleBar().Height());
			}

			bool IsVisible() const
			{
				// TODO
				return true;
			}

			double LeftInset() const
			{
				return static_cast<double>(m_AppWindow.TitleBar().LeftInset());
			}

			double RightInset() const
			{
				return static_cast<double>(m_AppWindow.TitleBar().RightInset());
			}

			auto ColorValues() const
			{
				return m_AppWindow.TitleBar();
			}

			com_array<IInspectable> BackingObjects() const
			{
				return { m_AppWindow.TitleBar() };
			}

			bool ExtendsContentIntoTitleBar() const
			{
				return m_AppWindow.TitleBar().ExtendsContentIntoTitleBar();
			}

			void ExtendsContentIntoTitleBar(bool value) const
			{
				m_AppWindow.TitleBar().ExtendsContentIntoTitleBar(value);
			}

			void ResetToDefault()
			{
				m_AppWindow.TitleBar().ResetToDefault();
			}
		private:
			AppWindow const m_AppWindow;
		};
	}
}