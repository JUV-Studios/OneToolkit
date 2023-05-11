#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::ThemeServiceDemo::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

	void MainPage::ComboBox_SelectionChanged(IInspectable const& sender, Controls::SelectionChangedEventArgs const& e)
	{
		auto selectedItemString = unbox_value<hstring>(e.AddedItems().GetAt(0));
		if (selectedItemString == L"Light")
		{
			OneToolkit::UI::Theming::ThemeService::Current().AppThemeOverride(OneToolkit::UI::Theming::RelativeTheme::Light);
		}
		else if (selectedItemString == L"Dark")
		{
			OneToolkit::UI::Theming::ThemeService::Current().AppThemeOverride(OneToolkit::UI::Theming::RelativeTheme::Dark);
		}
		else
		{
			OneToolkit::UI::Theming::ThemeService::Current().AppThemeOverride(OneToolkit::UI::Theming::RelativeTheme::Default);
		}
	}
}