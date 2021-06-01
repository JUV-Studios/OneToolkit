﻿//
// HubPanel.xaml.cpp
// Implementation of the HubPanel class
//

#include "pch.h"
#include "HubPanel.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Automation;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::UI::Controls;

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
	if (UniformMargin != value)
	{
		Padding = ThicknessHelper::FromLengths(value, value, value, 0);
		SetPaddingForSections();
		PropertyChanged(this, ref new PropertyChangedEventArgs("UniformMargin"));
	}
}

void HubPanel::Hub_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	SetAutomationProperties();
}

void HubPanel::SetPaddingForSections()
{
	auto value = UniformMargin;
	for (auto const& section : Sections) section->Padding = ThicknessHelper::FromLengths(value, 0, value, value);
}

void HubPanel::SetAutomationProperties()
{
	if (m_LastSetSize != Sections->Size)
	{
		for (uint32_t index = 0; index < Sections->Size; ++index)
		{
			auto section = Sections->GetAt(index);
			AutomationProperties::SetPositionInSet(section, index + 1);
			AutomationProperties::SetSizeOfSet(section, Sections->Size);
		}

		m_LastSetSize = Sections->Size;
	}

	SetPaddingForSections();
}

void HubPanel::Hub_SectionsInViewChanged(Object^ sender, SectionsInViewChangedEventArgs^ e)
{
	SetAutomationProperties();
}