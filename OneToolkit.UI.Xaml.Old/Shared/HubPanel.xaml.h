//
// HubPanel.xaml.h
// Declaration of the HubPanel class
//

#pragma once
#include "HubPanel.g.h"
#include "AttachedProperties.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HubPanel sealed
	{
	public:
		HubPanel();

		declare_dependency_property(double, Spacing);

		declare_dependency_property(bool, UseAutoOrientation);

		declare_dependency_property(XamlFramework::DataTemplate^, ItemTemplate);
	protected:
		Windows::Foundation::Size MeasureOverride(Windows::Foundation::Size availableSize) override;
	private:
		Windows::Foundation::EventRegistrationToken m_SizeChangedToken;
		void Hub_SizeChanged(Platform::Object^ sender, XamlFramework::SizeChangedEventArgs^ e);
		void OrientationPropertyChanged(XamlFramework::DependencyObject^ sender, XamlFramework::DependencyProperty^ property);
		static void DependencyPropertyChanged(XamlFramework::DependencyObject^ sender, XamlFramework::DependencyPropertyChangedEventArgs^ e);
	};
}