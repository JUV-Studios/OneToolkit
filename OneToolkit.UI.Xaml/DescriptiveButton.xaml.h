//
// DescriptiveButton.xaml.h
// Declaration of the DescriptiveButton class
//

#pragma once
#include "DescriptiveButton.g.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Markup::ContentProperty(Name = "Title")]
	public ref class DescriptiveButton sealed
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

		static property Windows::UI::Xaml::DependencyProperty^ TitleProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ CaptionProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}
	private:
		Platform::String^ m_Caption;
		static Windows::UI::Xaml::DependencyProperty^ m_TitleProperty;
		static Windows::UI::Xaml::DependencyProperty^ m_CaptionProperty;
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void SetProperties();
	};
}
