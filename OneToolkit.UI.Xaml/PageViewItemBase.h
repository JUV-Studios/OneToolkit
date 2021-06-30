#pragma once

namespace OneToolkit::UI::Xaml::Controls
{
	ref class PageView;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PageViewItemBase : MUXC::NavigationViewItem
	{
	public:
		property Platform::String^ Label
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}
	internal:
		virtual void Invoke(PageView^ sender) {}
	};
}