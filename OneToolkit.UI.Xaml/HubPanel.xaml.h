//
// HubPanel.xaml.h
// Declaration of the HubPanel class
//

#pragma once
#include "HubPanel.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HubPanel sealed
	{
	public:
		HubPanel();

		DeclareDependencyProperty(double, Spacing);

		DeclareDependencyProperty(bool, UseAutoOrientation);
		
		void SetPaddingProperties();
	private:
		void Hub_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void OrientationPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyProperty^ dp);
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
	};
}