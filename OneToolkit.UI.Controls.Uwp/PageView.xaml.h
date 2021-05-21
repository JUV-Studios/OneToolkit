//
// PageView.xaml.h
// Declaration of the PageView class
//

#pragma once
#include "PageView.g.h"
#include "PageViewLinkItem.h"

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewCustomItemInvokedHandler(PageView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewCustomSelectionChangedHandler(PageView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "DisplayContent")]
	public ref struct PageViewContentItem sealed : Microsoft::UI::Xaml::Controls::NavigationViewItem, IPageViewItem
	{
		virtual property Platform::String^ Label
		{
			Platform::String^ get()
			{
				return Content->ToString();
			}

			void set(Platform::String^ value)
			{
				if (Content != value)
				{
					Content = value;
					PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs("Label"));
				}
			}
		}

		property Windows::UI::Xaml::UIElement^ DisplayContent;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	};

	/// <summary>
	/// Extends NavigationView to handle content navigation automatically.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageView sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		PageView();

		/// <summary>
		/// The content that's currently displayed inside the control.
		/// </summary>
		property Windows::UI::Xaml::UIElement^ CurrentContent
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		/// <summary>
		/// The content that will be displayed when the user clicks on the settings navigation item.
		/// </summary>
		property Windows::UI::Xaml::UIElement^ SettingsContent
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		property int SelectedIndex
		{
			int get();
			void set(int value);
		}

		/// <summary>
		/// Gets or sets whether the custom drag region is enabled or not when the view is extended into the title bar. The custom drag region automatically adjusts the navigation pane so you can click on elements there without interference with the title bar drag region.
		/// </summary>
		property bool IsDragRegionEnabled
		{
			bool get();
			void set(bool value);
		}

		property bool UseDefaultTransitions
		{
			bool get();
			void set(bool value);
		}

		/// <summary>
		/// Fired when the PageView control can't decide what to do when an item is invoked. Handle this event to perform custom navigation/actions.
		/// </summary>
		event PageViewCustomItemInvokedHandler^ CustomItemInvoked;

		/// <summary>
		/// Fired when the PageView control can't decide what to do when an item is selected. Handle this event to perform custom navigation/actions.
		/// </summary>
		event PageViewCustomSelectionChangedHandler^ CustomSelectionChanged;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	private:
		bool m_Loaded;
		int m_IndexAfterLoad = -1;
		bool m_IsDragRegionEnabled;
		bool m_UseDefaultTransitions = true;
		Windows::UI::Xaml::UIElement^ m_SettingsContent;
		void SetDragRegionProperties();
		void NavigationView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void NavigationView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);
		void NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
	};
}