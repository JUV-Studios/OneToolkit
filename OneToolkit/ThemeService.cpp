#include "pch.h"
#include "UI.ThemeService.g.h"

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::ViewManagement;

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		struct ThemeService : ThemeServiceT<ThemeService>
		{
			static ApplicationTheme SystemAppsTheme()
			{
				return UISettings().GetColorValue(UIColorType::Background) == Colors::Black() ? ApplicationTheme::Dark : ApplicationTheme::Light;
			}

			static ApplicationTheme SystemShellTheme()
			{
				throw hresult_not_implemented();
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

void* winrt_make_OneToolkit_UI_ThemeService()
{
	return detach_abi(make<OneToolkit::UI::factory_implementation::ThemeService>());
}