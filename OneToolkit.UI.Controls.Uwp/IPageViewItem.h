#pragma once

namespace OneToolkit::UI::Controls
{
	public interface struct IPageViewItem : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
		property Platform::String^ Label
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}
	};
}