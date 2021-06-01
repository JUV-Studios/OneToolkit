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
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::UI::Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

HubPanelSection::HubPanelSection()
{
	InitializeComponent();
}

UIElement^ HubPanelSection::Content::get()
{
	return m_Content;
}

void HubPanelSection::Content::set(UIElement^ value)
{
	if (m_Content != value)
	{
		m_Content = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs("Content"));
	}
}