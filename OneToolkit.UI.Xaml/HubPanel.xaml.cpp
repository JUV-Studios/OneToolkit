//
// HubPanel.xaml.cpp
// Implementation of the HubPanel class
//

#include "pch.h"
#include "HubPanelSection.xaml.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Markup;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Automation;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DeclareDependencyProperty(double, HubPanel, Spacing, PropertyValue::CreateDouble(36));

HubPanel::HubPanel()
{
	InitializeComponent();
}

double HubPanel::UniformMargin::get()
{
	return Padding.Left;
}

void HubPanel::UniformMargin::set(double value)
{
	if (Padding.Left != value)
	{
		Padding = ThicknessHelper::FromLengths(value, value, value, 0);
		SetProperties();
		PropertyChanged(this, ref new PropertyChangedEventArgs("UniformMargin"));
	}
}

String^ HubPanel::Title::get()
{
	if (auto string = dynamic_cast<IBox<String^>^>(Header)) return string->Value;
	else if (auto stringable = dynamic_cast<IStringable^>(Header)) return stringable->ToString();
	else return "";
}

void HubPanel::Title::set(String^ value)
{
	if (Title != value)
	{
		Header = value;
		SetProperties();
		PropertyChanged(this, ref new PropertyChangedEventArgs("Title"));
	}
}

double HubPanel::Spacing::get()
{
	return static_cast<double>(GetValue(m_SpacingProperty));
}

void HubPanel::Spacing::set(double value)
{
	SetValue(m_SpacingProperty, value);
}

double HubPanel::HeaderSpacing::get()
{
	if (Application::Current->Resources->HasKey("HubPanelHeaderSpacing"))
	{
		if (auto resource = dynamic_cast<IBox<double>^>(Application::Current->Resources->Lookup("HubPanelHeaderSpacing"))) return resource->Value;
		else return 4;
	}
	else
	{
		return 4;
	}
}

void HubPanel::Hub_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	SetProperties();
}

void HubPanel::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	dynamic_cast<HubPanel^>(sender)->SetProperties();
}

void HubPanel::SetProperties()
{
	auto uniformMargin = Padding.Left;
	auto headerSpacing = HeaderSpacing;
	for (uint32 index = 0; index < Sections->Size; ++index)
	{
		auto section = Sections->GetAt(index);
		AutomationProperties::SetPositionInSet(section, index + 1);
		AutomationProperties::SetSizeOfSet(section, Sections->Size);
		if (Sections->Size <= 1)
		{
			section->Padding = ThicknessHelper::FromLengths(uniformMargin, headerSpacing, uniformMargin, uniformMargin);
		}
		else
		{
			if (FlowDirection == ::FlowDirection::LeftToRight)
			{
				if (index == 0) section->Padding = ThicknessHelper::FromLengths(uniformMargin, headerSpacing, Spacing, uniformMargin);
				else if (index == Sections->Size - 1) section->Padding = ThicknessHelper::FromLengths(0, headerSpacing, uniformMargin, uniformMargin);
				else section->Padding == ThicknessHelper::FromLengths(0, headerSpacing, Spacing, uniformMargin);
			}
			else
			{
				if (index == 0) section->Padding = ThicknessHelper::FromLengths(Spacing, headerSpacing, uniformMargin, uniformMargin);
				else if (index == Sections->Size - 1) section->Padding = ThicknessHelper::FromLengths(uniformMargin, headerSpacing, 0, uniformMargin);
				else section->Padding == ThicknessHelper::FromLengths(Spacing, headerSpacing, 0, uniformMargin);
			}
		}

		if (auto hubPanelSection = dynamic_cast<HubPanelSection^>(section)) hubPanelSection->Container = this;
	}
}

void HubPanel::Hub_SectionHeaderClick(Object^, HubSectionHeaderClickEventArgs^ e)
{
	if (auto section = dynamic_cast<HubPanelSection^>(e->Section)) section->RaiseHeaderClick(this);
}

void HubPanel::Hub_SectionsInViewChanged(Object^ sender, SectionsInViewChangedEventArgs^ e)
{
	SetProperties();
}