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
	public delegate void HubPanelSectionHeaderClickEventHandler(HubPanel^ sender);

	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "Content")]
	public ref class HubPanelSection sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
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

		event HubPanelSectionHeaderClickEventHandler^ HeaderClick;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	internal:
		property HubPanel^ Container
		{
			HubPanel^ get();
			void set(HubPanel^ value);
		}

		void RaiseHeaderClick(HubPanel^ sender);
	private:
		Platform::WeakReference m_Container;
		Windows::UI::Xaml::UIElement^ m_Content;
		void SetProperties();
	};
}