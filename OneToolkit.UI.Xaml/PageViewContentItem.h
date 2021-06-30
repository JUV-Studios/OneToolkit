#pragma once
#include "PageViewItemBase.h"
#include "IContentEquatable.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "SelectionContent")]
	public ref class PageViewContentItem sealed : PageViewItemBase, IContentEquatable
	{
	public:
		property Windows::UI::Xaml::UIElement^ SelectionContent;
		virtual bool Equals(Windows::UI::Xaml::UIElement^ content);
	internal:
		virtual void Invoke(PageView^ sender) override;
	};
}