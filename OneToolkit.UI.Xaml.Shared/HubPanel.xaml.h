//
// HubPanel.xaml.h
// Declaration of the HubPanel class
//

#pragma once
#include "HubPanel.g.h"
#include "AttachedProperties.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HubPanel sealed
	{
	public:
		HubPanel();

		declare_dependency_property(double, Spacing);

		declare_dependency_property(bool, UseAutoOrientation);
		
		void SetPaddingValues();
	private:
		void Hub_SizeChanged(Platform::Object^ sender, AppFramework::SizeChangedEventArgs^ e);
		void OrientationPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyProperty^ dp);
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e);
	};
}