//
// PageView.xaml.cpp
// Implementation of the PageView class
//

#include "pch.h"
#include "PageView.xaml.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::Mvvm;
using namespace OneToolkit::UI::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

PageView::PageView()
{
	InitializeComponent();
	IsTitleBarAutoPaddingEnabled = false;
}

UIElement^ PageView::CurrentContent::get()
{
	return dynamic_cast<UIElement^>(PageContent->Content);
}

void PageView::CurrentContent::set(UIElement^ value)
{
	if (CurrentContent != value)
	{
		PropertyChanging(this, ref new PropertyChangingEventArgs("CurrentContent"));
		PageContent->Content = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs("CurrentContent"));
	}
}

UIElement^ PageView::SettingsContent::get()
{
	return m_SettingsContent;
}

void PageView::SettingsContent::set(UIElement^ value)
{
	if (m_SettingsContent != value)
	{
		if (SelectedItem != nullptr && SelectedItem == SettingsItem) CurrentContent = value;
		m_SettingsContent = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs("SettingsContent"));
	}
}

bool PageView::IsDragRegionEnabled::get()
{
	return m_IsDragRegionEnabled;
}

void PageView::IsDragRegionEnabled::set(bool value)
{
	if (m_IsDragRegionEnabled != value)
	{
		m_IsDragRegionEnabled = value;
		SetDragRegionProperties();
		PropertyChanged(this, ref new PropertyChangedEventArgs("IsDragRegionEnabled"));
	}
}

void PageView::SetDragRegionProperties()
{
	if (m_IsDragRegionEnabled)
	{
		DragRegion->Height = 32;
		DragRegion->Visibility = Windows::UI::Xaml::Visibility::Visible;
		Window::Current->SetTitleBar(DragRegion);
	}
	else
	{
		DragRegion->Height = 0;
		DragRegion->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		Window::Current->SetTitleBar(nullptr);
	}
}

void PageView::NavigationView_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	SetDragRegionProperties();
}

void PageView::NavigationView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	// Handle navigation in selection changed for items that selects on invoked and others here
	auto navViewItem = dynamic_cast<Microsoft::UI::Xaml::Controls::NavigationViewItem^>(args->InvokedItemContainer);
	if (navViewItem != nullptr ? !navViewItem->SelectsOnInvoked : true)
	{
		if (args->IsSettingsInvoked && SettingsContent != nullptr) CurrentContent = SettingsContent;
		else if (auto pageViewItem = dynamic_cast<IPageViewItem^>(args->InvokedItemContainer)) pageViewItem->Invoke(this);
		else CustomItemInvoked(this, args);
	}
}

void PageView::NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args)
{
	if (args->IsSettingsSelected && SettingsContent != nullptr) CurrentContent = SettingsContent;
	else if (auto pageViewItem = dynamic_cast<IPageViewItem^>(args->SelectedItemContainer)) pageViewItem->Invoke(this);
	else CustomSelectionChanged(this, args);
}