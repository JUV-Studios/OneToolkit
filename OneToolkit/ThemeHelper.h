#pragma once
#include "UI.ThemeHelper.g.h"

namespace winrt::OneToolkit::UI::factory_implementation
{
	struct ThemeHelper : ThemeHelperT<ThemeHelper, ThemeHelper>
	{
		static Windows::UI::Xaml::ApplicationTheme SystemTheme();
	};
}