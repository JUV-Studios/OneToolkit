//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	auto const view = OneToolkit::UI::Windowing::ViewService::CurrentView;
	view->Title = L"Test";
	view->TitleBar->ExtendsContentIntoTitleBar = true;
	view->TitleBar->ButtonBackgroundColor = Windows::UI::Colors::Transparent;
	view->TitleBar->BackgroundColor = Windows::UI::Colors::Transparent;
}


void App1::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	OneToolkit::UI::Windowing::ViewService::CurrentView->MinimizeAsync();
}


void App1::MainPage::About_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//JuvApps::Common::AboutDialog().ShowAsync();
}
