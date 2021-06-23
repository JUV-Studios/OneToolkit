#pragma once
#include "PageViewItemBase.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct PageViewLinkItem sealed : PageViewItemBase
	{
	public:
		PageViewLinkItem();

		property Windows::System::LauncherOptions^ LauncherOptions
		{
			Windows::System::LauncherOptions^ get();
		}

		property Windows::Foundation::Uri^ NavigateUri;

		void Invoke(PageView^ sender) override;
	private:
		concurrency::task<void> InvokeAsync(PageView^ sender);
		Windows::System::LauncherOptions^ m_LauncherOptions = ref new Windows::System::LauncherOptions();
	};
}