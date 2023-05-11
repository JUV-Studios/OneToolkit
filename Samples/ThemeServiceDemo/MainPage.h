#pragma once
#include <winrt/OneToolkit.UI.Theming.h>
#include "MainPage.g.h"

namespace winrt::ThemeServiceDemo::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        void ComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
    };
}

namespace winrt::ThemeServiceDemo::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}

namespace winrt
{
	inline hstring to_hstring(OneToolkit::UI::Theming::AbsoluteTheme absoluteTheme)
	{
		return absoluteTheme == OneToolkit::UI::Theming::AbsoluteTheme::Dark ? L"Dark" : L"Light";
	}
}