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
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::UI;
using namespace OneToolkit::UI::Xaml::Controls;
using namespace OneToolkit::UI::Xaml::Media::Animation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DeclareDependencyProperty(bool, PageView, SyncBackWithSystem, true);

DeclareDependencyProperty(UIElement, PageView, SettingsContent, nullptr);

DeclareDependencyProperty(bool, PageView, IsNavigationStackEnabled, true);

DeclareDependencyProperty(IContentTransition, PageView, ContentTransition, nullptr);

PageView::PageView()
{
	InitializeComponent();
}

bool PageView::SyncBackWithSystem::get()
{
	return static_cast<bool>(GetValue(m_SyncBackWithSystemProperty));
}

void PageView::SyncBackWithSystem::set(bool value)
{
	SetValue(m_SyncBackWithSystemProperty, value);
}

bool PageView::IsNavigationStackEnabled::get()
{
	return static_cast<bool>(GetValue(m_IsNavigationStackEnabledProperty));
}

void PageView::IsNavigationStackEnabled::set(bool value)
{
	SetValue(m_IsNavigationStackEnabledProperty, value);
}

UIElement^ PageView::SettingsContent::get()
{
	return dynamic_cast<UIElement^>(GetValue(m_SettingsContentProperty));
}

void PageView::SettingsContent::set(UIElement^ value)
{
	SetValue(m_SettingsContentProperty, value);
}

IContentTransition^ PageView::ContentTransition::get()
{
	return dynamic_cast<IContentTransition^>(GetValue(m_ContentTransitionProperty));
}

void PageView::ContentTransition::set(IContentTransition^ value)
{
	SetValue(m_ContentTransitionProperty, value);
}

MUXC::NavigationViewItemBase^ PageView::ItemFromContent(UIElement^ content)
{
	if (!content) return nullptr;
	if (content == SettingsContent)
	{
		return dynamic_cast<MUXC::NavigationViewItemBase^>(SettingsItem);
	}
	else
	{
		auto found = FindContentItem(MenuItems, content);
		if (found == nullptr) found = FindContentItem(FooterMenuItems, content);
		return found;
	}
}

void PageView::GoBack()
{
	Frame->GoBack();
}

bool PageView::Navigate(UIElement^ content)
{
	return Frame->Navigate(content);
}

bool PageView::Navigate(TypeName sourcePageType)
{
	return Frame->Navigate(sourcePageType);
}

bool PageView::NavigateToType(TypeName typeName)
{
	return Frame->NavigateToType(typeName);
}

void PageView::InvokeItem(MUXC::NavigationViewItemBase^ navViewitem)
{
	if (auto pageViewItemBase = dynamic_cast<PageViewItemBase^>(navViewitem)) pageViewItemBase->Invoke(this);
}

void PageView::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto pageView = dynamic_cast<PageView^>(sender);
	if (e->Property == m_SettingsContentProperty)
	{
		if (pageView->SelectedItem != nullptr && pageView->SelectedItem == pageView->SettingsItem)
		{
			pageView->ContentTransition->SetContent(pageView->Frame, dynamic_cast<UIElement^>(e->NewValue), NavigationType::None);
		}
	}
}

MUXC::NavigationViewItemBase^ PageView::FindContentItem(IVector<Object^>^ collection, UIElement^ content)
{
	for (auto const& item : collection)
	{
		if (auto contentEquatable = dynamic_cast<IContentEquatable^>(static_cast<Object^>(item)))
		{
			if (contentEquatable->Equals(content)) return dynamic_cast<MUXC::NavigationViewItem^>(contentEquatable);
		}
	}

	return nullptr;
}

void PageView::OnPropertyChanged(Object^ sender, PropertyChangedEventArgs^ e)
{
	if (e->PropertyName == "CanGoBack") IsBackEnabled = Frame->CanGoBack;
	else if (e->PropertyName == "Content")
	{
		auto currentItem = ItemFromContent(dynamic_cast<UIElement^>(Frame->Content));
		if (dynamic_cast<IContentEquatable^>(currentItem)) SelectedItem = currentItem;
	}
}

void PageView::NavigationView_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	m_PropertyChangedToken = Frame->PropertyChanged += ref new PropertyChangedEventHandler(this, &PageView::OnPropertyChanged);
}

void PageView::NavigationView_Unloaded(Object^ sender, RoutedEventArgs^ e)
{
	Frame->PropertyChanged -= m_PropertyChangedToken;
}

void PageView::NavigationView_ItemInvoked(MUXC::NavigationView^, MUXC::NavigationViewItemInvokedEventArgs^ args)
{
	// Handle navigation in selection changed for items that selects on invoked and others here
	m_LastInvokedArgs = args;
	auto navViewItem = dynamic_cast<MUXC::NavigationViewItem^>(args->InvokedItemContainer);
	if (navViewItem != nullptr ? !navViewItem->SelectsOnInvoked : true)
	{
		if (args->IsSettingsInvoked && SettingsContent != nullptr) Navigate(SettingsContent);
		else if (auto pageViewItem = dynamic_cast<PageViewItemBase^>(args->InvokedItemContainer)) pageViewItem->Invoke(this);
		else MenuItemInvoked(this, args);
	}
}

void PageView::NavigationView_SelectionChanged(MUXC::NavigationView^ sender, MUXC::NavigationViewSelectionChangedEventArgs^ args)
{
	if (args->IsSettingsSelected && SettingsContent != nullptr)
	{
		Navigate(SettingsContent);
	}
	else if (auto pageViewItem = dynamic_cast<PageViewItemBase^>(args->SelectedItemContainer))
	{
		pageViewItem->Invoke(this);
	}
	else
	{
		MenuItemInvoked(this, m_LastInvokedArgs);
		MenuSelectionChanged(this, args);
		m_LastInvokedArgs = nullptr;
	}
}

void PageView::NavigationView_BackRequested(MUXC::NavigationView^ sender, MUXC::NavigationViewBackRequestedEventArgs^ args)
{
	Frame->GoBack();
}