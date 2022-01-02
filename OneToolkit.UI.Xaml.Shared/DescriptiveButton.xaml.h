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

		declare_dependency_property(AppFramework::UIElement^, Visuals);

		declare_dependency_property(Windows::Foundation::Uri^, NavigateUri);

		declare_dependency_property(AppFramework::Controls::Orientation, Orientation);
	private:
		Windows::Foundation::EventRegistrationToken m_ClickHandlerToken;
		void Button_Click(Object^ sender, AppFramework::RoutedEventArgs^ e);
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e);
	};
}
