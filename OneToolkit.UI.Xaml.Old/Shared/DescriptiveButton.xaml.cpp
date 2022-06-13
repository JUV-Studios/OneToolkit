//
// DescriptiveButton.xaml.cpp
// Implementation of the DescriptiveButton class
//

#include "pch.h"
#include "DescriptiveButton.xaml.h"

using namespace Platform;
using namespace XamlFramework;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace XamlFramework::Controls;
using namespace XamlFramework::Automation;
using namespace OneToolkit::Data::Text;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

define_dependency_property(String^, DescriptiveButton, Title, "");

define_dependency_property(String^, DescriptiveButton, Caption, "");

define_dependency_property(UIElement^, DescriptiveButton, Visuals, nullptr);

define_dependency_property(::Orientation, DescriptiveButton, Orientation, ::Orientation::Horizontal);

DescriptiveButton::DescriptiveButton()
{
	InitializeComponent();
}

void DescriptiveButton::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<DescriptiveButton^>(sender);
	if (e->Property == m_TitleProperty || e->Property == m_CaptionProperty)
	{
		std::wstring automationName;
		if (!StringExtensions::HasOnlyWhitespaces(target->Title))
		{
			automationName = target->Title->Data();
			target->TitleBlock->Visibility = ::Visibility::Visible;
		}
		else
		{
			target->TitleBlock->Visibility = ::Visibility::Collapsed;
		}

		if (!StringExtensions::HasOnlyWhitespaces(target->Caption))
		{
			if (!automationName.empty()) automationName += L", ";
			automationName += target->Title->Data();
			target->CaptionBlock->Visibility = ::Visibility::Visible;
		}
		else
		{
			target->CaptionBlock->Visibility = ::Visibility::Collapsed;
		}

		AutomationProperties::SetName(target, ref new String(automationName.data(), static_cast<uint32>(automationName.size())));
	}
}