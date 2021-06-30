//
// DescriptiveButton.xaml.cpp
// Implementation of the DescriptiveButton class
//

#include "pch.h"
#include "DescriptiveButton.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Automation;
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DeclareDependencyProperty(String, DescriptiveButton, Title, "");

DeclareDependencyProperty(String, DescriptiveButton, Caption, "");

DescriptiveButton::DescriptiveButton()
{
	InitializeComponent();
}

void DescriptiveButton::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<DescriptiveButton^>(sender);
	AutomationProperties::SetName(target, target->Title + L", " + target->Caption);
}

String^ DescriptiveButton::Title::get()
{
	return GetValue(m_TitleProperty)->ToString();
}

void DescriptiveButton::Title::set(String^ value)
{
	SetValue(m_TitleProperty, value);
}

String^ DescriptiveButton::Caption::get()
{
	return GetValue(m_CaptionProperty)->ToString();
}

void DescriptiveButton::Caption::set(String^ value)
{
	SetValue(m_CaptionProperty, value);
}