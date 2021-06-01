//
// PageView.xaml.h
// Declaration of the PageView class
//

#pragma once
#include "PageView.g.h"
#include "PageViewLinkItem.h"
#include "PageViewContentItem.h"

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewCustomItemInvokedHandler(PageView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewCustomSelectionChangedHandler(PageView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
	
	/// <summary>
	/// Extends NavigationView to handle content navigation automatically.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageView sealed : OneToolkit::Mvvm::INotifyPropertyChanging, Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		PageView();

		property Windows::UI::Xaml::UIElement^ CurrentContent
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		property Windows::UI::Xaml::UIElement^ SettingsContent
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		property bool IsDragRegionEnabled
		{
			bool get();
			void set(bool value);
		}

		event PageViewCustomItemInvokedHandler^ CustomItemInvoked;

		event PageViewCustomSelectionChangedHandler^ CustomSelectionChanged;

		virtual event OneToolkit::Mvvm::PropertyChangingEventHandler^ PropertyChanging;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	private:
		bool m_IsDragRegionEnabled;
		bool m_UseDefaultTransitions = true;
		Windows::UI::Xaml::UIElement^ m_SettingsContent;
		void SetDragRegionProperties();
		void NavigationView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void NavigationView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);
		void NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
	};
}