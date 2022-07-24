#pragma once
#include "XamlFramework/Base.h"
#include "DependencyProperty.h"
#include "Controls.DescriptiveButton.g.h"
#include <winrt/Windows.Foundation.h>

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