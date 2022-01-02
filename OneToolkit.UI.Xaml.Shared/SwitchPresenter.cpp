// Ported from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/SwitchPresenter.cs & https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI.Controls.Primitives/SwitchPresenter/Case.cs.

#include "pch.h"
#include "SwitchPresenter.h"

using namespace Platform;
using namespace AppFramework;
using namespace Windows::UI::Xaml::Interop;
using namespace OneToolkit::UI::Xaml::Controls;

define_dependency_property(bool, Case, IsDefault, false);

define_dependency_property(Object^, Case, Value, nullptr);

define_dependency_property(UIElement^, Case, Content, nullptr);

define_dependency_property(Object^, SwitchPresenter, Value, nullptr);

DependencyProperty^ SwitchPresenter::m_CurrentCaseProperty = DependencyProperty::Register("CurrentCase", Case::typeid, SwitchPresenter::typeid, nullptr);

SwitchPresenter::SwitchPresenter()
{
	Loaded += ref new RoutedEventHandler(this, &OneToolkit::UI::Xaml::Controls::SwitchPresenter::SwitchPresenter_Loaded);
}

Case^ SwitchPresenter::CurrentCase::get()
{
	return static_cast<Case^>(GetValue(m_CurrentCaseProperty));
}

DependencyObjectCollection^ SwitchPresenter::SwitchCases::get()
{
	return %m_SwitchCases;
}

DependencyProperty^ SwitchPresenter::CurrentCaseProperty::get()
{
	return m_CurrentCaseProperty;
}

void SwitchPresenter::OnApplyTemplate()
{
	ContentPresenter::OnApplyTemplate();
	EvaluateCases();
}

void SwitchPresenter::EvaluateCases()
{
	if (m_SwitchCases.Size == 0)
	{
		// If we have no cases, then we can't match anything.
		if (CurrentCase)
		{
			// Only bother clearing our actual content if we had something before.
			Content = nullptr;
			SetValue(m_CurrentCaseProperty, nullptr);
		}
	}
	else
	{
		if (CurrentCase)
		{
			if (CurrentCase->Value == Value)
			{
				// If the current case we're on already matches our current value, then we don't have any work to do.
				return;
			}
		}
		
		Case^ newCase;
		Case^ defaultCase;
		for (auto object : SwitchCases)
		{
			auto switchCase = static_cast<Case^>(static_cast<DependencyObject^>(object));
			if (switchCase->IsDefault)
			{
				// If there are multiple default cases provided, this will override and just grab the last one, the developer will have to fix this in their XAML. We call this out in the case comments.
				defaultCase = switchCase;
			}
			else if (switchCase->Value->Equals(Value))
			{
				newCase = switchCase;
			}
		}

		if (!newCase && defaultCase)
		{
			// Inject default if we found one without matching anything
			newCase = defaultCase;
		}

		// Only bother changing things around if we actually have a new case.
		if (newCase != CurrentCase)
		{
			// If we don't have any cases or default, setting these to null is what we want to be blank again.
			Content = newCase ? newCase->Content : nullptr;
			SetValue(m_CurrentCaseProperty, newCase);
		}
	}
}

void SwitchPresenter::SwitchPresenter_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	// In case we're in a template, we may have loaded cases later.
	EvaluateCases();
}

void SwitchPresenter::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	static_cast<SwitchPresenter^>(sender)->EvaluateCases();
}