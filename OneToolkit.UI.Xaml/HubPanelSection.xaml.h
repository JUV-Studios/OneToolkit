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

		property Windows::UI::Xaml::UIElement^ Content
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}
	internal:
		property HubPanel^ Container
		{
			HubPanel^ get();
			void set(HubPanel^ value);
		}
	private:
		Platform::WeakReference m_Content;
		Platform::WeakReference m_Container;
		Platform::WeakReference m_Presenter;
		void SetProperties();
		void ContentPresenter_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void DependencyPropertyBaseChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyProperty^ dp);
	};
}