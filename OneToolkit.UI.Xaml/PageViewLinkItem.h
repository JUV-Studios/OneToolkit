#pragma once
#include "PageViewItemBase.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageViewLinkItem sealed : PageViewItemBase
	{
	public:
		PageViewLinkItem();

		property Windows::Foundation::Uri^ NavigateUri;
	internal:
		void Invoke(PageView^ sender) override;
	private:
		concurrency::task<void> InvokeAsync();
	};
}