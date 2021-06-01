//
// DescriptiveButton.xaml.h
// Declaration of the DescriptiveButton class
//

#pragma once
#include "DescriptiveButton.g.h"

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DescriptiveButton sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		DescriptiveButton();

		property Platform::String^ Title
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property Platform::String^ Caption
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	private:
		Platform::String^ m_Title;
		Platform::String^ m_Caption;
	};
}
