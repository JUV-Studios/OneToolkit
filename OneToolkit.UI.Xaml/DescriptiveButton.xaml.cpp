//
// DescriptiveButton.xaml.cpp
// Implementation of the DescriptiveButton class
//

#include "pch.h"
#include "DescriptiveButton.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Automation;
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::UI::Xaml;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DefineDependencyProperty(String^, DescriptiveButton, Title, "");

DefineDependencyProperty(String^, DescriptiveButton, Caption, "");

DefineDependencyProperty(Uri^, DescriptiveButton, NavigateUri, nullptr);

DescriptiveButton::DescriptiveButton()
{
	InitializeComponent();
}

void DescriptiveButton::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<DescriptiveButton^>(sender);
	AutomationProperties::SetName(target, target->Title + L", " + target->Caption);
	if (e->Property == m_NavigateUriProperty)
	{
		if (e->NewValue) target->m_ClickHandlerToken = target->Click += ref new RoutedEventHandler(target, &DescriptiveButton::Button_Click);
		else target->Click -= target->m_ClickHandlerToken;
	}
}

void DescriptiveButton::Button_Click(Object^ sender, RoutedEventArgs^ e)
{
	Launcher::LaunchUriAsync(NavigateUri);
}