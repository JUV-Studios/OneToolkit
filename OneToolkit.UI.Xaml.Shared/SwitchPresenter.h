// Ported from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/SwitchPresenter.cs & https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/Case.cs.

#pragma once

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Framework::Markup::ContentProperty(Name = "Content")]
	public ref class Case sealed : Framework::DependencyObject
	{
	public:
		declare_dependency_property(bool, IsDefault);

		declare_dependency_property(Platform::Object^, Value);

		declare_dependency_property(Framework::UIElement^, Content);
	private:
		static void DependencyPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyPropertyChangedEventArgs^ e) {}
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	[Framework::Markup::ContentProperty(Name = "SwitchCases")]
	public ref class SwitchPresenter sealed : Framework::Controls::ContentPresenter
	{
	public:
		SwitchPresenter();

		declare_dependency_property(Platform::Object^, Value);

		property Case^ CurrentCase
		{
			Case^ get();
		}

		property Framework::DependencyObjectCollection^ SwitchCases
		{
			Framework::DependencyObjectCollection^ get();
		}

		static property Framework::DependencyProperty^ CurrentCaseProperty
		{
			Framework::DependencyProperty^ get();
		}
	protected:
		void OnApplyTemplate() override;
	private:
		Framework::DependencyObjectCollection m_SwitchCases;
		void EvaluateCases();
		void SwitchPresenter_Loaded(Platform::Object^ sender, Framework::RoutedEventArgs^ e);
		static Framework::DependencyProperty^ m_CurrentCaseProperty;
		static void DependencyPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyPropertyChangedEventArgs^ e);
	};
}