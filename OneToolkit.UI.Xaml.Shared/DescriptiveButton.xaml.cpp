//
// DescriptiveButton.xaml.cpp
// Implementation of the DescriptiveButton class
//

#include "pch.h"
#include "DescriptiveButton.xaml.h"

using namespace Platform;
using namespace Framework;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Framework::Automation;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

define_dependency_property(String^, DescriptiveButton, Title, "");

define_dependency_property(String^, DescriptiveButton, Caption, "");

define_dependency_property(UIElement^, DescriptiveButton, Visuals, nullptr);

define_dependency_property(Uri^, DescriptiveButton, NavigateUri, nullptr);

DescriptiveButton::DescriptiveButton()
{
	InitializeComponent();
}

void DescriptiveButton::Button_Click(Object^ sender, RoutedEventArgs^ e)
{
	Launcher::LaunchUriAsync(NavigateUri);
}

void DescriptiveButton::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<DescriptiveButton^>(sender);
	AutomationProperties::SetName(target, target->Title + ", " + target->Caption);
	if (e->Property == m_NavigateUriProperty)
	{
		if (e->NewValue) target->m_ClickHandlerToken = target->Click += ref new RoutedEventHandler(target, &DescriptiveButton::Button_Click);
		else target->Click -= target->m_ClickHandlerToken;
	}
}