module;

#include "WindowingBase.h"
#include <winrt/OneToolkit.UI.Windowing.h>

#define color_value_accessor(name) Windows::Foundation::IReference<Windows::UI::Color> name() const { return static_cast<Derived const&>(*this).ColorValues().name(); }\
void name(Windows::Foundation::IReference<Windows::UI::Color> const& value) const { static_cast<Derived const&>(*this).ColorValues().name(value); }

export module WindowTitleBar;

using namespace juv;

export namespace winrt::OneToolkit::UI::Windowing::implementation
{
	template <typename Derived, typename... Interfaces>
	struct WindowTitleBarBase : implements<Derived, IWindowTitleBar>
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
		struct WindowTitleBar : WindowTitleBarBase<WindowTitleBar>
		{
			WindowTitleBar(WindowTitleBarController const& controller) : Controller(controller) {}

			auto_property<WindowTitleBarController> const Controller;

			auto ColorValues() const
			{
				return Controller().AppViewTitleBar();
			}

			double Height() const
			{
				return Controller().CoreAppViewTitleBar().Height();
			}

			bool IsVisible() const
			{
				return Controller().CoreAppViewTitleBar().IsVisible();
			}

			double LeftInset() const
			{
				return Controller().CoreAppViewTitleBar().SystemOverlayLeftInset();
			}

			double RightInset() const
			{
				return Controller().CoreAppViewTitleBar().SystemOverlayRightInset();
			}

			bool ExtendsContentIntoTitleBar() const
			{
				return Controller().CoreAppViewTitleBar().ExtendViewIntoTitleBar();
			}

			void ExtendsContentIntoTitleBar(bool value) const
			{
				Controller().CoreAppViewTitleBar().ExtendViewIntoTitleBar(value);
			}
		};
	}

	namespace Universal::Preview
	{
		struct WindowTitleBar : WindowTitleBarBase<WindowTitleBar>
		{
			WindowTitleBar(AppWindowTitleBar const& controller) : Controller(controller) {}

			auto_property<AppWindowTitleBar> const Controller;

			double Height() const
			{
				// TOOD return Controller().GetTitleBarOcclusions().GetAt(0).OccludingRect().Height;
				return 32;
			}

			bool IsVisible() const
			{
				return Controller().IsVisible();
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
				return Controller();
			}

			bool ExtendsContentIntoTitleBar() const
			{
				return Controller().ExtendsContentIntoTitleBar();
			}

			void ExtendsContentIntoTitleBar(bool value) const
			{
				Controller().ExtendsContentIntoTitleBar(value);
			}
		};
	}

	namespace Desktop
	{
		struct WindowTitleBar : WindowTitleBarBase<WindowTitleBar>
		{
			WindowTitleBar(AppWindowTitleBar const& controller) : Controller(controller) {}

			auto_property<AppWindowTitleBar> const Controller;

			double Height() const
			{
				return static_cast<double>(Controller().Height());
			}

			bool IsVisible() const
			{
				// TODO
				return true;
			}

			double LeftInset() const
			{
				return static_cast<double>(Controller().LeftInset());
			}

			double RightInset() const
			{
				return static_cast<double>(Controller().RightInset());
			}

			auto ColorValues() const
			{
				return Controller();
			}

			bool ExtendsContentIntoTitleBar() const
			{
				return Controller().ExtendsContentIntoTitleBar();
			}

			void ExtendsContentIntoTitleBar(bool value) const
			{
				Controller().ExtendsContentIntoTitleBar(value);
			}

			void ResetToDefault()
			{
				Controller().ResetToDefault();
			}
		};
	}
}