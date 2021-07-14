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

		DeclareDependencyProperty(Windows::UI::Xaml::UIElement^, Content);
	internal:
		property HubPanel^ Container
		{
			HubPanel^ get();
			void set(HubPanel^ value);
		}
	private:
		Platform::WeakReference m_Container;
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void SetProperties();
		void DependencyPropertyBaseChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyProperty^ dp);
	};
}