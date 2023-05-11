#include "pch.h"
#include "DescriptiveButton.h"
#include "DescriptiveButton.g.cpp"

using namespace winrt;
using namespace XamlFramework;
using namespace XamlFramework::Controls;

namespace winrt::OneToolkit::UI::Xaml::Controls::implementation
{
	DependencyProperty const DescriptiveButton::s_TitleProperty = DependencyProperty::Register(L"Title", xaml_typename<hstring>(), xaml_typename<OneToolkit::UI::Xaml::Controls::DescriptiveButton>(), nullptr);

	DependencyProperty const DescriptiveButton::s_CaptionProperty = DependencyProperty::Register(L"Caption", xaml_typename<hstring>(), xaml_typename<OneToolkit::UI::Xaml::Controls::DescriptiveButton>(), nullptr);

	DependencyProperty const DescriptiveButton::s_VisualsProperty = DependencyProperty::Register(L"Visuals", xaml_typename<IInspectable>(), xaml_typename<OneToolkit::UI::Xaml::Controls::DescriptiveButton>(), nullptr);

	DependencyProperty const DescriptiveButton::s_OrientationProperty = DependencyProperty::Register(L"Orientation", xaml_typename<::Orientation>(), xaml_typename<OneToolkit::UI::Xaml::Controls::DescriptiveButton>(), PropertyMetadata(box_value(::Orientation::Horizontal)));

    DescriptiveButton::DescriptiveButton()
    {
        InitializeComponent();
    }
}