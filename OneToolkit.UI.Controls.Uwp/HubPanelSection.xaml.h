//
// HubPanelSection.xaml.h
// Declaration of the HubPanelSection class
//

#pragma once
#include "HubPanelSection.g.h"

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "Content")]
	public ref class HubPanelSection sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		HubPanelSection();

		property Windows::UI::Xaml::UIElement^ Content
		{
			Windows::UI::Xaml::UIElement^ get();
			void set(Windows::UI::Xaml::UIElement^ value);
		}

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	private:
		Windows::UI::Xaml::UIElement^ m_Content;
	};
}
