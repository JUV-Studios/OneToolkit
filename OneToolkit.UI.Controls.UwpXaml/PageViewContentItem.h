#pragma once
#include "IPageViewItem.h"

namespace OneToolkit::UI::Controls
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "DisplayContent")]
	public ref struct PageViewContentItem sealed : Microsoft::UI::Xaml::Controls::NavigationViewItem, IPageViewItem
	{
		virtual property Platform::String^ Label
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property Windows::UI::Xaml::UIElement^ DisplayContent;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		virtual void Invoke(PageView^ sender);
	};
}