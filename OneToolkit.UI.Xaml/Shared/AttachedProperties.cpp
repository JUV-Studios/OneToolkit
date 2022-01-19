#include "pch.h"
#include "AttachedProperties.h"

using namespace Platform;
using namespace XamlFramework;
using namespace XamlFramework::Controls;
using namespace Windows::Foundation;
using namespace OneToolkit::UI::Xaml;

std::map<WeakReference, EventRegistrationToken> appBarClosingRegistrations;

void SetSpacingForItems(ListViewBase^ listView, double oldSpacing, double newSpacing)
{
	for (auto const& item : listView->Items)
	{
		if (auto itemContainer = listView->ItemsPanelRoot ? dynamic_cast<FrameworkElement^>(listView->ContainerFromItem(item)) : dynamic_cast<FrameworkElement^>(static_cast<Object^>(item)))
		{
			auto margin = itemContainer->Margin;
			margin.Right -= oldSpacing;
			margin.Bottom -= oldSpacing;
			margin.Right += newSpacing;
			margin.Bottom += newSpacing;
			itemContainer->Margin = margin;
		}
	}
}

DependencyProperty^ AttachedProperties::SpacingProperty::get()
{
	return m_Spacing.Property;
}

DependencyProperty^ AttachedProperties::IsCollapsibleProperty::get()
{
	return m_IsCollapsible.Property;
}

double AttachedProperties::GetSpacing(ListViewBase^ listView)
{
	return m_Spacing(listView);
}

void AttachedProperties::SetSpacing(ListViewBase^ listView, double value)
{
	m_Spacing(listView, value);
}

bool AttachedProperties::GetIsCollapsible(AppBar^ appBar)
{
	return m_IsCollapsible(appBar);
}

void AttachedProperties::SetIsCollapsible(AppBar^ appBar, bool value)
{
	m_IsCollapsible(appBar, value);
}

void AttachedProperties::Spacing_ValueChanged(DependencyObject^ dependencyObject, double oldValue, double newValue)
{
	auto listView = dynamic_cast<ListViewBase^>(dependencyObject);

	// Set a margin with the spacing value for each item in the view.
	SetSpacingForItems(listView, oldValue, newValue);

	// Set a negative margin on the view to avoid extra space after the last row/column.
	auto margin = listView->Margin;
	margin.Right -= oldValue;
	margin.Bottom -= oldValue;
	margin.Right += -newValue;
	margin.Bottom += -newValue;
	listView->Margin = margin;

	listView->ChoosingItemContainer += ref new TypedEventHandler<ListViewBase^, ChoosingItemContainerEventArgs^>(&AttachedProperties::OnChoosingItemContainer);
}

void AttachedProperties::IsCollapsible_ValueChanged(DependencyObject^ dependencyObject, bool oldValue, bool newValue)
{
	auto appBar = dynamic_cast<AppBar^>(dependencyObject);
	WeakReference appBarWeak{ appBar };
	if (newValue)
	{
		if (auto iterator = appBarClosingRegistrations.find(appBarWeak); iterator != appBarClosingRegistrations.end()) appBar->Closing -= iterator->second;
	}
	else
	{
		appBar->IsOpen = true;
		appBar->IsSticky = true;
		if (auto commandBar = dynamic_cast<CommandBar^>(appBar)) commandBar->OverflowButtonVisibility = CommandBarOverflowButtonVisibility::Collapsed;
		appBarClosingRegistrations[appBarWeak] = appBar->Closing += ref new EventHandler<Object^>(&AttachedProperties::OnAppBarClosing);
	}
}
void AttachedProperties::OnAppBarClosing(Object^ sender, Object^ args)
{
	dynamic_cast<AppBar^>(sender)->IsOpen = true;
}


void AttachedProperties::OnChoosingItemContainer(XamlFramework::Controls::ListViewBase^ sender, XamlFramework::Controls::ChoosingItemContainerEventArgs^ args)
{
	if (args->IsContainerPrepared)
	{
		auto newSpacing = GetSpacing(sender);
		auto const childContainer = dynamic_cast<FrameworkElement^>(static_cast<UIElement^>(args->ItemContainer));
		auto margin = childContainer->Margin;
		margin.Right += newSpacing;
		margin.Bottom += newSpacing;
		childContainer->Margin = margin;
	}
}