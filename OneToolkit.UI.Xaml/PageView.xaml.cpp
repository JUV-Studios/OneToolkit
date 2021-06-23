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
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::Mvvm;
using namespace OneToolkit::UI;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DependencyProperty^ PageView::m_IsDragRegionEnabledProperty = DependencyProperty::Register("IsDragRegionEnabled", bool::typeid, PageView::typeid, 
	ref new PropertyMetadata(false, ref new PropertyChangedCallback(&PageView::OnPropertyChanged)));

PageView::PageView()
{
	InitializeComponent();
	IsTitleBarAutoPaddingEnabled = false;
}

IViewServiceProvider^ PageView::ViewServiceProvider::get()
{
	if (m_ViewServiceProvider == nullptr) m_ViewServiceProvider = ViewService::GetForCurrentView();
	return m_ViewServiceProvider;
}

void PageView::ViewServiceProvider::set(IViewServiceProvider^ value)
{
	m_ViewServiceProvider = value;
}

UIElement^ PageView::CurrentContent::get()
{
	return dynamic_cast<UIElement^>(PageContent->Content);
}

void PageView::CurrentContent::set(UIElement^ value)
{
	if (CurrentContent != value)
	{
		PropertyChangingEventArgs changingArgs;
		changingArgs.PropertyName = "CurrentContent";
		PropertyChanging(this, changingArgs);
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
	return static_cast<bool>(GetValue(m_IsDragRegionEnabledProperty));
}

void PageView::IsDragRegionEnabled::set(bool value)
{
	SetValue(m_IsDragRegionEnabledProperty, value);
}

bool PageView::IsDragRegionApplied::get()
{
	return IsDragRegionEnabled && ViewServiceProvider->InteractionMode == UserInteractionMode::Mouse && !ViewServiceProvider->IsFullScreen;
}

DependencyProperty^ PageView::IsDragRegionEnabledProperty::get()
{
	return m_IsDragRegionEnabledProperty;
}

void PageView::OnPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	if (e->Property == m_IsDragRegionEnabledProperty) dynamic_cast<PageView^>(sender)->SetDragRegionProperties();
}

void PageView::SetDragRegionProperties()
{
	if (IsDragRegionApplied)
	{
		auto titleBarHeight = CoreApplication::GetCurrentView()->TitleBar->Height;
		DragRegion->Height = titleBarHeight == 0 ? 32 : titleBarHeight;
		DragRegion->Visibility = Windows::UI::Xaml::Visibility::Visible;
		Window::Current->SetTitleBar(DragRegion);
	}
	else
	{
		DragRegion->Height = 0;
		DragRegion->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		Window::Current->SetTitleBar(nullptr);
	}

	PropertyChanged(this, ref new PropertyChangedEventArgs("IsDragRegionApplied"));
}

void PageView::NavigationView_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	SetDragRegionProperties();
}

void PageView::NavigationView_SizeChanged(Object^ sender, SizeChangedEventArgs^ e)
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
		else if (auto pageViewItem = dynamic_cast<PageViewItemBase^>(args->InvokedItemContainer)) pageViewItem->Invoke(this);
		else MenuItemInvoked(this, args);
	}
}

void PageView::NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args)
{
	if (args->IsSettingsSelected && SettingsContent != nullptr) CurrentContent = SettingsContent;
	else if (auto pageViewItem = dynamic_cast<PageViewItemBase^>(args->SelectedItemContainer)) pageViewItem->Invoke(this);
	else MenuSelectionChanged(this, args);
}