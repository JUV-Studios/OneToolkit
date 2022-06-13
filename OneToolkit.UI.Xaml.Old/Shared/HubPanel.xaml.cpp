//
// HubPanel.xaml.cpp
// Implementation of the HubPanel class
//

#include "pch.h"
#include "HubPanel.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace XamlFramework;
using namespace XamlFramework::Controls;
using namespace XamlFramework::Automation;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

define_dependency_property(double, HubPanel, Spacing, 36.0);

define_dependency_property(bool, HubPanel, UseAutoOrientation, true);

define_dependency_property(DataTemplate^, HubPanel, ItemTemplate, nullptr);

HubPanel::HubPanel()
{
	InitializeComponent();
	m_SizeChangedToken = SizeChanged += ref new SizeChangedEventHandler(this, &HubPanel::Hub_SizeChanged);
	RegisterPropertyChangedCallback(OrientationProperty, ref new DependencyPropertyChangedCallback(this, &HubPanel::OrientationPropertyChanged));
}

Size HubPanel::MeasureOverride(Size availableSize)
{
	std::vector<HubSection^> displayedSections;
	std::copy_if(begin(Sections), end(Sections), std::back_inserter(displayedSections), [](auto const& section)
		{
			if (section->Visibility == ::Visibility::Collapsed)
			{
				section->Padding = ThicknessHelper::FromUniformLength(0);
				return false;
			}

			return true;
		});

	auto const headerBottomPadding = GetResourceValue(Resources, "HubPanelHeaderBottomPadding", 12.0);
	for (size_t index = 0; index < displayedSections.size(); ++index)
	{
		auto section = displayedSections[index];
		AutomationProperties::SetSizeOfSet(section, static_cast<int>(Sections->Size));
		AutomationProperties::SetPositionInSet(section, static_cast<int>(index + 1));
		if (displayedSections.size() == 1) section->Padding = ThicknessHelper::FromLengths(Padding.Left, headerBottomPadding, Padding.Right, Padding.Top);
		else if (Orientation == ::Controls::Orientation::Horizontal)
		{
			if (index == 0) section->Padding = ThicknessHelper::FromLengths(Padding.Left, headerBottomPadding, 0, Padding.Top);
			else if (index == displayedSections.size() - 1) section->Padding = ThicknessHelper::FromLengths(Spacing, headerBottomPadding, Padding.Right, Padding.Top);
			else section->Padding = ThicknessHelper::FromLengths(Spacing, headerBottomPadding, 0, Padding.Top);
		}
		else
		{
			if (index == 0) section->Padding = ThicknessHelper::FromLengths(Padding.Left, 0, Padding.Right, 0);
			else if (index == displayedSections.size() - 1) section->Padding = ThicknessHelper::FromLengths(Padding.Left, Spacing, Padding.Right, Padding.Top);
			else section->Padding = ThicknessHelper::FromLengths(Padding.Left, Spacing, Padding.Right, 0);
		}
	}

	return Hub::MeasureOverride(availableSize);
}

void HubPanel::Hub_SizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	Orientation = e->NewSize.Width >= GetResourceValue(Resources, "MediumWindowBreakpoint", 641.0) ? ::Controls::Orientation::Horizontal : ::Controls::Orientation::Vertical;
}

void HubPanel::OrientationPropertyChanged(DependencyObject^ sender, DependencyProperty^ property)
{
	UpdateLayout();
}

void HubPanel::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<HubPanel^>(sender);
	if (e->Property == m_SpacingProperty) target->UpdateLayout();
	else if (e->Property == m_UseAutoOrientationProperty)
	{
		if (static_cast<bool>(e->NewValue)) target->m_SizeChangedToken = target->SizeChanged += ref new SizeChangedEventHandler(target, &HubPanel::Hub_SizeChanged);
		else target->SizeChanged -= target->m_SizeChangedToken;
	}
}