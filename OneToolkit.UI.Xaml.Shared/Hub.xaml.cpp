//
// Hub.xaml.cpp
// Implementation of the Hub class
//

#include "pch.h"
#include "Hub.xaml.h"

using namespace Framework;
using namespace Framework::Automation;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

define_dependency_property(double, Hub, Spacing, 36.0);

define_dependency_property(bool, Hub, UseAutoOrientation, true);

Hub::Hub()
{
	InitializeComponent();
	RegisterPropertyChangedCallback(OrientationProperty, ref new DependencyPropertyChangedCallback(this, &Hub::OrientationPropertyChanged));
}

void Hub::SetPaddingProperties()
{
	std::vector<Framework::Controls::HubSection^> visibleSections;
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

void Hub::Hub_SizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	double mediumWindowBreakpoint = 641;
	if (Resources->HasKey("MediumWindowBreakpoint")) mediumWindowBreakpoint = static_cast<double>(Resources->Lookup("MediumWindowBreakpoint"));
	if (UseAutoOrientation) Orientation = e->NewSize.Width >= mediumWindowBreakpoint ? ::Controls::Orientation::Horizontal : ::Controls::Orientation::Vertical;
}

void Hub::OrientationPropertyChanged(DependencyObject^ sender, DependencyProperty^ dp)
{
	SetPaddingProperties();
}

void Hub::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	dynamic_cast<Hub^>(sender)->SetPaddingProperties();
}