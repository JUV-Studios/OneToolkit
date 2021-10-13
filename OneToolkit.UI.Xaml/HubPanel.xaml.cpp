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
using namespace OneToolkit::UI::Converters;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DefineDependencyProperty(double, HubPanel, Spacing, 36.0);

DefineDependencyProperty(bool, HubPanel, UseAutoOrientation, true);

HubPanel::HubPanel()
{
	InitializeComponent();
	RegisterPropertyChangedCallback(OrientationProperty, ref new DependencyPropertyChangedCallback(this, &HubPanel::OrientationPropertyChanged));
}

void HubPanel::SetPaddingProperties()
{
	std::vector<HubSection^> visibleSections;
	for (auto const& section : Sections)
	{
		if (section->Visibility == ::Visibility::Visible && section->Opacity != 0) visibleSections.emplace_back(section);
	}

	for (size_t index = 0; index < visibleSections.size(); ++index)
	{
		auto section = visibleSections[index];
		AutomationProperties::SetPositionInSet(section, static_cast<int>(index + 1));
		AutomationProperties::SetSizeOfSet(section, static_cast<int>(visibleSections.size()));
		if (visibleSections.size() == 1) section->Padding = Thickness(Padding.Left, 0, Padding.Right, Padding.Top);
		else if (Orientation == ::Orientation::Horizontal)
		{
			if (index == 0) section->Padding = Thickness(Padding.Left, 0, 0, Padding.Top);
			else if (index == visibleSections.size() - 1) section->Padding = Thickness(Spacing, 0, Padding.Right, Padding.Top);
			else section->Padding = Thickness(Spacing, 0, 0, Padding.Top);
		}
		else
		{
			if (index == 0) section->Padding = Thickness(Padding.Left, 0, Padding.Right, 0);
			else if (index == visibleSections.size() - 1) section->Padding = Thickness(Padding.Left, Spacing, Padding.Right, Padding.Top);
			else section->Padding = Thickness(Padding.Left, Spacing, Padding.Right, 0);
		}
	}
}

void HubPanel::Hub_SizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	if (UseAutoOrientation) Orientation = e->NewSize.Width > 640 ? ::Orientation::Horizontal : ::Orientation::Vertical;
}

void HubPanel::OrientationPropertyChanged(DependencyObject^ sender, DependencyProperty^ dp)
{
	SetPaddingProperties();
}

void HubPanel::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	dynamic_cast<HubPanel^>(sender)->SetPaddingProperties();
}