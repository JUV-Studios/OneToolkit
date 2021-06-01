#pragma once
#include "IPageViewItem.h"

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void PageViewLinkItemAfterLinkLaunchedHandler(bool result);

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentPropertyAttribute(Name = "Label")]
	public ref struct PageViewLinkItem sealed : Microsoft::UI::Xaml::Controls::NavigationViewItem, IPageViewItem
	{
		PageViewLinkItem();

		virtual property Platform::String^ Label
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property Windows::System::LauncherOptions^ Options;

		property Windows::Foundation::Uri^ NavigateUri;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		virtual void Invoke(PageView^ sender);
	};
}