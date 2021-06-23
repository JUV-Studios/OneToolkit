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
	try
	{
		return static_cast<String^>(Header);
	}
	catch (InvalidCastException^)
	{
		if (auto stringable = dynamic_cast<IStringable^>(Header)) return Header->ToString();
		else return "";
	}
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

void HubPanel::Hub_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	SetProperties();
}

void HubPanel::SetProperties()
{
	auto uniformMargin = Padding.Left;
	for (uint32 index = 0; index < Sections->Size; ++index)
	{
		auto section = Sections->GetAt(index);
		AutomationProperties::SetPositionInSet(section, index + 1);
		AutomationProperties::SetSizeOfSet(section, Sections->Size);
		section->Padding = ThicknessHelper::FromLengths(uniformMargin, 6, uniformMargin, uniformMargin);
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