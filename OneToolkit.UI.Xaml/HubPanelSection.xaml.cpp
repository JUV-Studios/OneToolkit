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

HubPanelSection::HubPanelSection()
{
	InitializeComponent();
	auto propertyChangedCallback = ref new DependencyPropertyChangedCallback(this, &HubPanelSection::DependencyPropertyBaseChanged);
	RegisterPropertyChangedCallback(HeaderProperty, propertyChangedCallback);
}

UIElement^ HubPanelSection::Content::get()
{
	return m_Content.Resolve<UIElement>();
}

void HubPanelSection::Content::set(UIElement^ value)
{
	if (Content != value)
	{
		m_Content = value;
		if (auto presenter = m_Presenter.Resolve<ContentPresenter>()) presenter->Content = value;
		SetProperties();
	}
}

HubPanel^ HubPanelSection::Container::get()
{
	return m_Container.Resolve<HubPanel>();
}

void HubPanelSection::Container::set(HubPanel^ value)
{
	if (Container != value) m_Container = value;
	SetProperties();
}

void HubPanelSection::SetProperties()
{
	if (auto content = Content)
	{
		auto automationText = Header->ToString();
		if (auto container = Container)
		{
			uint32 foundIndex;
			if (container->Sections->IndexOf(this, &foundIndex))
			{
				if (foundIndex == 0) automationText = container->Header->ToString() + L", " + automationText;
			}
		}

		AutomationProperties::SetName(content, automationText);
	}
}

void HubPanelSection::ContentPresenter_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	auto target = dynamic_cast<ContentPresenter^>(sender);
	if (!target->Content) target->Content = Content;
	m_Presenter = target;
}

void HubPanelSection::DependencyPropertyBaseChanged(DependencyObject^ sender, DependencyProperty^ dp)
{
	SetProperties();
}