//
// PageView.xaml.h
// Declaration of the PageView class
//

#pragma once
#include "PageView.g.h"
#include "ContentFrame.xaml.h"
#include "PageViewLinkItem.h"
#include "PageViewContentItem.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewMenuItemInvokedHandler(PageView^ sender, MUXC::NavigationViewItemInvokedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewMenuSelectionChangedHandler(PageView^ sender, MUXC::NavigationViewSelectionChangedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageView sealed
	{
	public:
		PageView();

		property bool SyncBackWithSystem
		{
			bool get();
			void set(bool value);
		}

		property Windows::UI::Xaml::UIElement^ SettingsContent
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		property Media::Animation::IContentTransition^ ContentTransition
		{
			Media::Animation::IContentTransition^ get();
			void set(Media::Animation::IContentTransition^ value);
		}

		static property Windows::UI::Xaml::DependencyProperty^ SettingsContentProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ ContentTransitionProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ SyncBackWithSystemProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		void GoBack();

		void GoForward();

		void Navigate(Windows::UI::Xaml::UIElement^ content);

		void NavigateToType(Windows::UI::Xaml::Interop::TypeName typeName);

		void InvokeItem(MUXC::NavigationViewItemBase^ navViewitem);

		event PageViewMenuItemInvokedHandler^ MenuItemInvoked;

		event PageViewMenuSelectionChangedHandler^ MenuSelectionChanged;

		MUXC::NavigationViewItemBase^ ItemFromContent(Windows::UI::Xaml::UIElement^ content);		
	private:
		MUXC::NavigationViewItemInvokedEventArgs^ m_LastInvokedArgs;
		Windows::Foundation::EventRegistrationToken m_PropertyChangedToken;
		static Windows::UI::Xaml::DependencyProperty^ m_SettingsContentProperty;
		static Windows::UI::Xaml::DependencyProperty^ m_ContentTransitionProperty;
		static Windows::UI::Xaml::DependencyProperty^ m_SyncBackWithSystemProperty;
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		MUXC::NavigationViewItemBase^ FindContentItem(Windows::Foundation::Collections::IVector<Platform::Object^>^ collection, Windows::UI::Xaml::UIElement^ content);
		void OnPropertyChanged(Platform::Object^ sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs^ e);
		void NavigationView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void NavigationView_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void NavigationView_ItemInvoked(MUXC::NavigationView^ sender, MUXC::NavigationViewItemInvokedEventArgs^ args);
		void NavigationView_SelectionChanged(MUXC::NavigationView^ sender, MUXC::NavigationViewSelectionChangedEventArgs^ args);
		void NavigationView_BackRequested(MUXC::NavigationView^ sender, MUXC::NavigationViewBackRequestedEventArgs^ args);
	};
}