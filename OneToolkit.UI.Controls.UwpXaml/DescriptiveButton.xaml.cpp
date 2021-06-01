//
// DescriptiveButton.xaml.cpp
// Implementation of the DescriptiveButton class
//

#include "pch.h"
#include "DescriptiveButton.xaml.h"

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

DescriptiveButton::DescriptiveButton()
{
	InitializeComponent();
}

String^ DescriptiveButton::Title::get()
{
	return m_Title;
}

void DescriptiveButton::Title::set(String^ value)
{
	if (m_Title != value)
	{
		m_Title = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs(L"Title"));
	}
}

String^ DescriptiveButton::Caption::get()
{
	return m_Caption;
}

void DescriptiveButton::Caption::set(String^ value)
{
	if (m_Caption != value)
	{
		m_Caption = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs(L"Caption"));
	}
}