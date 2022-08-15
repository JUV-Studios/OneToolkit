#pragma once
#include "DescriptiveButton.g.h"

namespace winrt::OneToolkit::UI::Xaml::Controls
{
	namespace implementation
	{
		struct DescriptiveButton : DescriptiveButtonT<DescriptiveButton>
		{
			DescriptiveButton();
			DeclareDependencyProperty(hstring, Title);
			DeclareDependencyProperty(hstring, Caption);
			DeclareDependencyProperty(IInspectable, Visuals);
			DeclareDependencyProperty(XamlFramework::Controls::Orientation, Orientation);
		};
	}

	namespace factory_implementation
	{
		struct DescriptiveButton : DescriptiveButtonT<DescriptiveButton, implementation::DescriptiveButton>
		{
		};
	}
}