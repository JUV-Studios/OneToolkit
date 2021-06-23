//
// PageView.xaml.h
// Declaration of the PageView class
//

#pragma once
#include "PageView.g.h"
#include "PageViewLinkItem.h"
#include "PageViewContentItem.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewMenuItemInvokedHandler(PageView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewMenuSelectionChangedHandler(PageView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
	
	/// <summary>
	/// Extends NavigationView to handle content navigation automatically.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageView sealed : OneToolkit::Mvvm::INotifyPropertyChanging, Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		PageView();

		property IViewServiceProvider^ ViewServiceProvider
		{
			IViewServiceProvider^ get();
			void set(IViewServiceProvider^ value);
		}

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

		property bool IsDragRegionApplied
		{
			bool get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ IsDragRegionEnabledProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		/*property bool SyncBackButtonWithSystem
		{
			bool get();
			void set(bool value);
		}*/

		event PageViewMenuItemInvokedHandler^ MenuItemInvoked;

		event PageViewMenuSelectionChangedHandler^ MenuSelectionChanged;

		virtual event OneToolkit::Mvvm::PropertyChangingEventHandler^ PropertyChanging;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	private:
		bool m_UseDefaultTransitions = true;
		IViewServiceProvider^ m_ViewServiceProvider;
		Windows::UI::Xaml::UIElement^ m_SettingsContent;
		static Windows::UI::Xaml::DependencyProperty^ m_IsDragRegionEnabledProperty;
		static void OnPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void SetDragRegionProperties();
		void NavigationView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void NavigationView_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void NavigationView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);
		void NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
	};
}