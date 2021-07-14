//
// HubPanelSection.xaml.cpp
// Implementation of the HubPanelSection class
//

#include "pch.h"
#include "HubPanelSection.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Automation;
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::UI::Xaml::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DefineDependencyProperty(UIElement, HubPanelSection, Content, nullptr);

HubPanelSection::HubPanelSection()
{
	InitializeComponent();
	RegisterPropertyChangedCallback(HeaderProperty, ref new DependencyPropertyChangedCallback(this, &HubPanelSection::DependencyPropertyBaseChanged));
}

UIElement^ HubPanelSection::Content::get()
{
	return static_cast<UIElement^>(GetValue(m_ContentProperty));
}

void HubPanelSection::Content::set(UIElement^ value)
{
	SetValue(m_ContentProperty, value);
}

HubPanel^ HubPanelSection::Container::get()
{
	return m_Container.Resolve<HubPanel>();
}

void HubPanelSection::Container::set(HubPanel^ value)
{
	if (Container != value)
	{
		m_Container = value;
		SetProperties();
	}
}

void HubPanelSection::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	dynamic_cast<HubPanelSection^>(sender)->SetProperties();
}

void HubPanelSection::SetProperties()
{
	if (Content != nullptr)
	{
		auto automationText = Header->ToString();
		if (auto container = m_Container.Resolve<HubPanel>())
		{
			uint32 foundIndex;
			if (container->Sections->IndexOf(this, &foundIndex))
			{
				if (foundIndex == 0) automationText = container->Header->ToString() + L", " + automationText;
			}
		}

		AutomationProperties::SetName(Content, automationText);
	}
}

void HubPanelSection::DependencyPropertyBaseChanged(DependencyObject^ sender, DependencyProperty^ dp)
{
	SetProperties();
}