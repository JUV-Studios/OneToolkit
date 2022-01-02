//
// HubPanel.xaml.cpp
// Implementation of the HubPanel class
//

#include "pch.h"
#include "HubPanel.xaml.h"

using namespace Platform;
using namespace AppFramework;
using namespace AppFramework::Automation;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

define_dependency_property(double, HubPanel, Spacing, 36.0);

define_dependency_property(bool, HubPanel, UseAutoOrientation, true);

HubPanel::HubPanel()
{
	InitializeComponent();
	//DefaultStyleKey = HubPanel::typeid->FullName;
	RegisterPropertyChangedCallback(OrientationProperty, ref new DependencyPropertyChangedCallback(this, &HubPanel::OrientationPropertyChanged));
}

void HubPanel::SetPaddingValues()
{
	std::vector<AppFramework::Controls::HubSection^> visibleSections;
	for (auto const& section : Sections)
	{
		if (section->Visibility == ::Visibility::Visible && section->Opacity != 0) visibleSections.emplace_back(section);
		else section->Padding = ThicknessHelper::FromUniformLength(0);
	}

	for (size_t index = 0; index < visibleSections.size(); ++index)
	{
		auto section = visibleSections[index];
		AutomationProperties::SetPositionInSet(section, static_cast<int>(index + 1));
		AutomationProperties::SetSizeOfSet(section, static_cast<int>(visibleSections.size()));
		if (visibleSections.size() == 1) section->Padding = ThicknessHelper::FromLengths(Padding.Left, 0, Padding.Right, Padding.Top);
		else if (Orientation == ::Controls::Orientation::Horizontal)
		{
			if (index == 0) section->Padding = ThicknessHelper::FromLengths(Padding.Left, 0, 0, Padding.Top);
			else if (index == visibleSections.size() - 1) section->Padding = ThicknessHelper::FromLengths(Spacing, 0, Padding.Right, Padding.Top);
			else section->Padding = ThicknessHelper::FromLengths(Spacing, 0, 0, Padding.Top);
		}
		else
		{
			if (index == 0) section->Padding = ThicknessHelper::FromLengths(Padding.Left, 0, Padding.Right, 0);
			else if (index == visibleSections.size() - 1) section->Padding = ThicknessHelper::FromLengths(Padding.Left, Spacing, Padding.Right, Padding.Top);
			else section->Padding = ThicknessHelper::FromLengths(Padding.Left, Spacing, Padding.Right, 0);
		}
	}
}

void HubPanel::Hub_SizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	if (UseAutoOrientation)
	{
		double mediumWindowBreakpoint = 641;
		if (Resources->HasKey("MediumWindowBreakpoint"))
		{
			if (auto propertyValue = dynamic_cast<IBox<double>^>(Resources->Lookup("MediumWindowBreakpoint"))) mediumWindowBreakpoint = propertyValue->Value;
		}

		Orientation = e->NewSize.Width >= mediumWindowBreakpoint ? ::Controls::Orientation::Horizontal : ::Controls::Orientation::Vertical;
	}
}

void HubPanel::OrientationPropertyChanged(DependencyObject^ sender, DependencyProperty^ dp)
{
	SetPaddingValues();
}

void HubPanel::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	dynamic_cast<HubPanel^>(sender)->SetPaddingValues();
}