//
// HubPanel.xaml.h
// Declaration of the HubPanel class
//

#pragma once
#include "HubPanel.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HubPanel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		HubPanel();

		DeclareDependencyProperty(double, Spacing);

		DeclareDependencyProperty(Windows::UI::Xaml::Thickness, Packing);

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	internal:
		void SetProperties();
	private:
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void Hub_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Hub_SectionsInViewChanged(Platform::Object^ sender, WUXC::SectionsInViewChangedEventArgs^ e);
		void DependencyPropertyBaseChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyProperty^ dependencyProperty);
	};
}