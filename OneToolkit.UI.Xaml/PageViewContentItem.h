#pragma once
#include "PageViewItemBase.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "SelectionContent")]
	public ref struct PageViewContentItem sealed : PageViewItemBase
	{
		property Windows::UI::Xaml::UIElement^ SelectionContent;

		virtual void Invoke(PageView^ sender) override;
	};
}