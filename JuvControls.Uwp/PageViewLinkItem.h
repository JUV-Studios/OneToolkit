#pragma once

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewLinkItemAfterLinkLaunchedHandler(bool result);

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageViewLinkItem sealed : Microsoft::UI::Xaml::Controls::NavigationViewItem
	{
	public:
		PageViewLinkItem();

		property Windows::System::LauncherOptions^ Options;

		property Windows::Foundation::Uri^ NavigateUri;

		event PageViewLinkItemAfterLinkLaunchedHandler^ AfterLinkLaunched;
	internal:
		void Raise(bool result);
	};
}