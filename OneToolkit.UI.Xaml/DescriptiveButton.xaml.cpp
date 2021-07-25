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
using namespace OneToolkit::UI::Xaml;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DefineDependencyProperty(String^, DescriptiveButton, Title, "");

DefineDependencyProperty(String^, DescriptiveButton, Caption, "");

DescriptiveButton::DescriptiveButton()
{
	InitializeComponent();
}

void DescriptiveButton::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<DescriptiveButton^>(sender);
	AutomationProperties::SetName(target, target->Title + L", " + target->Caption);
	target->TitleBlock->Visibility = target->Title ? ::Visibility::Visible : ::Visibility::Collapsed;
	target->CaptionBlock->Visibility = target->Caption ? ::Visibility::Visible : ::Visibility::Collapsed;
}