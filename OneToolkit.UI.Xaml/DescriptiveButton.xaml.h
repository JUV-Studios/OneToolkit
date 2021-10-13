//
// DescriptiveButton.xaml.h
// Declaration of the DescriptiveButton class
//

#pragma once
#include "DescriptiveButton.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "Title")]
	public ref class DescriptiveButton sealed
	{
	public:
		DescriptiveButton();

		DeclareDependencyProperty(Platform::String^, Title);

		DeclareDependencyProperty(Platform::String^, Caption);
		
		DeclareDependencyProperty(Windows::Foundation::Uri^, NavigateUri);
	private:
		Windows::Foundation::EventRegistrationToken m_ClickHandlerToken;
		void Button_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
	};
}
