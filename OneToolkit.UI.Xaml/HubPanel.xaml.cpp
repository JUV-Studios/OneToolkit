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

HubPanel::HubPanel()
{
	InitializeComponent();
	RegisterPropertyChangedCallback(HeaderProperty, ref new DependencyPropertyChangedCallback(this, &HubPanel::DependencyPropertyBaseChanged));
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
	for (uint32 index = 0; index < Sections->Size; ++index)
	{
		auto section = Sections->GetAt(index);
		AutomationProperties::SetPositionInSet(section, index + 1);
		AutomationProperties::SetSizeOfSet(section, Sections->Size);
		if (Sections->Size == 1) section->Padding = ThicknessHelper::FromLengths(Padding.Left, 0, Padding.Right, Padding.Top);
		else if (index == 0) section->Padding = ThicknessHelper::FromLengths(Padding.Left, 0, 0, Padding.Bottom);
		else if (index == Sections->Size - 1) section->Padding = ThicknessHelper::FromLengths(Spacing, 0, Padding.Right, Padding.Top);
		else section->Padding = ThicknessHelper::FromLengths(Spacing, 0, 0, Padding.Top);
	}
}

void HubPanel::Hub_SectionsInViewChanged(Object^ sender, SectionsInViewChangedEventArgs^ e)
{
	SetProperties();
}

void HubPanel::DependencyPropertyBaseChanged(DependencyObject^ sender, DependencyProperty^ dependencyProperty)
{
	SetProperties();
}