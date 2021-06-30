//
// HubPanelSection.xaml.h
// Declaration of the HubPanelSection class
//

#pragma once
#include "HubPanel.xaml.h"
#include "HubPanelSection.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "Content")]
	public ref class HubPanelSection sealed
	{
	public:
		HubPanelSection();

		property Platform::String^ Title
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property Windows::UI::Xaml::UIElement^ Content
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		static property Windows::UI::Xaml::DependencyProperty^ ContentProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}
	internal:
		property HubPanel^ Container
		{
			HubPanel^ get();
			void set(HubPanel^ value);
		}
	private:
		Platform::WeakReference m_Container;
		static Windows::UI::Xaml::DependencyProperty^ m_ContentProperty;
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void SetProperties();
	};
}