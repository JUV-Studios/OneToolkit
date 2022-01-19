// Ported from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/SwitchPresenter.cs & https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/Case.cs.

#pragma once

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[XamlFramework::Markup::ContentProperty(Name = "Content")]
	public ref class Case sealed : XamlFramework::DependencyObject
	{
	public:
		declare_dependency_property(bool, IsDefault);

		declare_dependency_property(Platform::Object^, Value);

		declare_dependency_property(XamlFramework::UIElement^, Content);
	private:
		static void DependencyPropertyChanged(XamlFramework::DependencyObject^ sender, XamlFramework::DependencyPropertyChangedEventArgs^ e) {}
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	[XamlFramework::Markup::ContentProperty(Name = "SwitchCases")]
	public ref class SwitchPresenter sealed : XamlFramework::Controls::ContentPresenter
	{
	public:
		SwitchPresenter();

		declare_dependency_property(Platform::Object^, Value);

		property Case^ CurrentCase
		{
			Case^ get();
		}

		property XamlFramework::DependencyObjectCollection^ SwitchCases
		{
			XamlFramework::DependencyObjectCollection^ get();
		}

		static property XamlFramework::DependencyProperty^ CurrentCaseProperty
		{
			XamlFramework::DependencyProperty^ get();
		}
	protected:
		void OnApplyTemplate() override;
	private:
		XamlFramework::DependencyObjectCollection m_SwitchCases;
		void EvaluateCases();
		void SwitchPresenter_Loaded(Platform::Object^ sender, XamlFramework::RoutedEventArgs^ e);
		static XamlFramework::DependencyProperty^ m_CurrentCaseProperty;
		static void DependencyPropertyChanged(XamlFramework::DependencyObject^ sender, XamlFramework::DependencyPropertyChangedEventArgs^ e);
	};
}