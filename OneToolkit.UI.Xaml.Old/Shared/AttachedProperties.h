#pragma once

namespace OneToolkit::UI::Xaml
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AttachedProperties sealed
	{
	public:
		static property XamlFramework::DependencyProperty^ SpacingProperty
		{
			XamlFramework::DependencyProperty^ get();
		}

		static property XamlFramework::DependencyProperty^ IsCollapsibleProperty
		{
			XamlFramework::DependencyProperty^ get();
		}

		static double GetSpacing(XamlFramework::Controls::ListViewBase^ listView);

		static void SetSpacing(XamlFramework::Controls::ListViewBase^ listView, double value);

		static bool GetIsCollapsible(XamlFramework::Controls::AppBar^ appBar);

		static void SetIsCollapsible(XamlFramework::Controls::AppBar^ appBar, bool value);
	private:
		AttachedProperties();
		static void Spacing_ValueChanged(XamlFramework::DependencyObject^ dependencyObject, double oldValue, double newValue);
		static void IsCollapsible_ValueChanged(XamlFramework::DependencyObject^ dependencyObject, bool oldValue, bool newValue);
		static void OnAppBarClosing(Platform::Object^ sender, Platform::Object^ args);
		static void OnChoosingItemContainer(XamlFramework::Controls::ListViewBase^ sender, XamlFramework::Controls::ChoosingItemContainerEventArgs^ args);
		inline static DependencyPropertyWrapper<double, AttachedProperties, DependencyPropertyKind::Attached> m_Spacing{ "Spacing", Spacing_ValueChanged };
		inline static DependencyPropertyWrapper<bool, AttachedProperties, DependencyPropertyKind::Attached> m_IsCollapsible{ "IsCollapsible", IsCollapsible_ValueChanged, true };
	};
}