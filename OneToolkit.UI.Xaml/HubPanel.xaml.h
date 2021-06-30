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

		property double UniformMargin
		{
			double get();
			void set(double value);
		}

		property Platform::String^ Title
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property double Spacing
		{
			double get();
			void set(double value);
		}

		property double HeaderSpacing
		{
			double get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ SpacingProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	private:
		uint32 m_LastSetSize = 0;
		static Windows::UI::Xaml::DependencyProperty^ m_SpacingProperty;
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void SetProperties();
		void Hub_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Hub_SectionsInViewChanged(Platform::Object^ sender, WUXC::SectionsInViewChangedEventArgs^ e);
	};
}