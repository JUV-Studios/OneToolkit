//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Showcase;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::UI::Controls;

namespace MUXC = Microsoft::UI::Xaml::Controls;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}
void MainPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
}
	
void MainPage::Page_SizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	float width = e->NewSize.Width;
	if (width <= 640)
	{
		if (Transitions->Size > 0 ? dynamic_cast<Animation::PaneThemeTransition^>(Transitions->GetAt(0)) == nullptr : true)
		{
			NavView->IsDragRegionEnabled = false;
			NavView->PaneDisplayMode = MUXC::NavigationViewPaneDisplayMode::Top;
			Transitions->Clear();
			Transitions->Append(ref new Animation::PaneThemeTransition);
			PaneTitleBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar = false;
		}
	}
	else
	{
		if (width >= 1008)
		{
			NavView->IsDragRegionEnabled = false;
			NavView->IsPaneToggleButtonVisible = false;
			NavView->PaneDisplayMode = MUXC::NavigationViewPaneDisplayMode::Left;
			PaneTitleBlock->Margin = ThicknessHelper::FromLengths(12, 12, 12, 0);
		}
		else
		{
			NavView->IsDragRegionEnabled = true;
			NavView->IsPaneToggleButtonVisible = true;
			NavView->PaneDisplayMode = MUXC::NavigationViewPaneDisplayMode::LeftCompact;
			PaneTitleBlock->Margin = ThicknessHelper::FromLengths(4, 12, 12, 0);
		}

		if (Transitions->Size > 0 ? dynamic_cast<Animation::EntranceThemeTransition^>(Transitions->GetAt(0)) == nullptr : true)
		{
			Transitions->Clear();
			Transitions->Append(ref new Animation::EntranceThemeTransition);
			PaneTitleBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
			CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar = true;
		}
	}
}