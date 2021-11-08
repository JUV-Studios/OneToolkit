//
// DescriptiveButton.xaml.h
// Declaration of the DescriptiveButton class
//

#pragma once
#include "Converters.h"
#include "DescriptiveButton.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "Title")]
	public ref class DescriptiveButton sealed
	{
	public:
		DescriptiveButton();

		declare_dependency_property(Platform::String^, Title);

		declare_dependency_property(Platform::String^, Caption);

		declare_dependency_property(Framework::UIElement^, Visuals);
		
		declare_dependency_property(Windows::Foundation::Uri^, NavigateUri);
	private:
		Windows::Foundation::EventRegistrationToken m_ClickHandlerToken;
		void Button_Click(Object^ sender, Framework::RoutedEventArgs^ e);
		static void DependencyPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyPropertyChangedEventArgs^ e);
	};
}
