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

DefineDependencyProperty(double, HubPanel, Spacing, PropertyValue::CreateDouble(36));

DefineDependencyProperty(Thickness, HubPanel, Packing, ThicknessHelper::FromUniformLength(48));

HubPanel::HubPanel()
{
	InitializeComponent();
	RegisterPropertyChangedCallback(HeaderProperty, ref new DependencyPropertyChangedCallback(this, &HubPanel::DependencyPropertyBaseChanged));
}

double HubPanel::Spacing::get()
{
	return static_cast<double>(GetValue(m_SpacingProperty));
}

void HubPanel::Spacing::set(double value)
{
	SetValue(m_SpacingProperty, value);
}

Thickness HubPanel::Packing::get()
{
	return static_cast<Thickness>(GetValue(m_PackingProperty));
}

void HubPanel::Packing::set(Thickness value)
{
	SetValue(m_PackingProperty, value);
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
	Thickness paddingValue;
	paddingValue.Left = Packing.Left;
	paddingValue.Top = Packing.Top;
	paddingValue.Right = Packing.Right;
	paddingValue.Bottom = 0;
	Padding = paddingValue;
	for (uint32 index = 0; index < Sections->Size; ++index)
	{
		auto section = Sections->GetAt(index);
		AutomationProperties::SetPositionInSet(section, index + 1);
		AutomationProperties::SetSizeOfSet(section, Sections->Size);
		if (Sections->Size == 1)
		{
			section->Padding = ThicknessHelper::FromLengths(Packing.Left, 0, Packing.Right, Packing.Bottom);
		}
		else
		{
			if (index == 0) section->Padding = ThicknessHelper::FromLengths(Packing.Left, 0, 0, Packing.Bottom);
			else if (index == Sections->Size - 1) section->Padding = ThicknessHelper::FromLengths(Spacing, 0, Packing.Right, Packing.Bottom);
			else section->Padding = ThicknessHelper::FromLengths(Spacing, 0, 0, Packing.Bottom);
		}

		if (auto hubPanelSection = dynamic_cast<HubPanelSection^>(section)) hubPanelSection->Container = this;
	}
}

void HubPanel::Hub_SectionsInViewChanged(Object^ sender, SectionsInViewChangedEventArgs^ e)
{
	SetProperties();
}

void HubPanel::DependencyPropertyBaseChanged(DependencyObject^ sender, DependencyProperty^ dp)
{
	SetProperties();
}