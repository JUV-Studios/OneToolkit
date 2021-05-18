//
// HubPanel.xaml.h
// Declaration of the HubPanel class
//

#pragma once
#include "HubPanel.g.h"

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HubPanel sealed
	{
	public:
		HubPanel();
	private:
		uint32_t m_LastSetSize = 0;
		void SetAutomationProperties();
		void Hub_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Hub_SectionsInViewChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SectionsInViewChangedEventArgs^ e);
	};
}