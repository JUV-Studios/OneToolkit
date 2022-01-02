// Ported from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/SwitchPresenter.cs & https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/Case.cs.

#pragma once

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[AppFramework::Markup::ContentProperty(Name = "Content")]
	public ref class Case sealed : AppFramework::DependencyObject
	{
	public:
		declare_dependency_property(bool, IsDefault);

		declare_dependency_property(Platform::Object^, Value);

		declare_dependency_property(AppFramework::UIElement^, Content);
	private:
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e) {}
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	[AppFramework::Markup::ContentProperty(Name = "SwitchCases")]
	public ref class SwitchPresenter sealed : AppFramework::Controls::ContentPresenter
	{
	public:
		SwitchPresenter();

		declare_dependency_property(Platform::Object^, Value);

		property Case^ CurrentCase
		{
			Case^ get();
		}

		property AppFramework::DependencyObjectCollection^ SwitchCases
		{
			AppFramework::DependencyObjectCollection^ get();
		}

		static property AppFramework::DependencyProperty^ CurrentCaseProperty
		{
			AppFramework::DependencyProperty^ get();
		}
	protected:
		void OnApplyTemplate() override;
	private:
		AppFramework::DependencyObjectCollection m_SwitchCases;
		void EvaluateCases();
		void SwitchPresenter_Loaded(Platform::Object^ sender, AppFramework::RoutedEventArgs^ e);
		static AppFramework::DependencyProperty^ m_CurrentCaseProperty;
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e);
	};
}