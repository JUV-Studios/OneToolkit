#pragma once

namespace OneToolkit::UI::Xaml
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AttachedProperties sealed
	{
	public:
		static property AppFramework::DependencyProperty^ SpacingProperty
		{
			AppFramework::DependencyProperty^ get();
		}

		static double GetSpacing(AppFramework::Controls::ListViewBase^ listView);
		static void SetSpacing(AppFramework::Controls::ListViewBase^ listView, double value);
	private:
		AttachedProperties();
		static AppFramework::DependencyProperty^ m_SpacingProperty;
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e);
		static void OnChoosingItemContainer(AppFramework::Controls::ListViewBase^ sender, AppFramework::Controls::ChoosingItemContainerEventArgs^ args);
	};
}