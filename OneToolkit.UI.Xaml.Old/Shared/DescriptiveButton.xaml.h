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

		declare_dependency_property(Platform::String^, Title);

		declare_dependency_property(Platform::String^, Caption);

		declare_dependency_property(XamlFramework::UIElement^, Visuals);

		declare_dependency_property(XamlFramework::Controls::Orientation, Orientation);
	private:
		static void DependencyPropertyChanged(XamlFramework::DependencyObject^ sender, XamlFramework::DependencyPropertyChangedEventArgs^ e);
	};
}
