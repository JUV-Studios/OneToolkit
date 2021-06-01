#pragma once

namespace OneToolkit::UI::Controls
{
	ref class PageView;

	[Windows::Foundation::Metadata::WebHostHidden]
	public interface struct IPageViewItem : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
		property Platform::String^ Label
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		void Invoke(PageView^ sender);
	};
}