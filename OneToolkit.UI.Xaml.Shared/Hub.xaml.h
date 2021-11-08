//
// Hub.xaml.h
// Declaration of the Hub class
//

#pragma once
#include "Hub.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Hub sealed
	{
	public:
		Hub();

		declare_dependency_property(double, Spacing);

		declare_dependency_property(bool, UseAutoOrientation);
		
		void SetPaddingProperties();
	private:
		void Hub_SizeChanged(Platform::Object^ sender, Framework::SizeChangedEventArgs^ e);
		void OrientationPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyProperty^ dp);
		static void DependencyPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyPropertyChangedEventArgs^ e);
	};
}