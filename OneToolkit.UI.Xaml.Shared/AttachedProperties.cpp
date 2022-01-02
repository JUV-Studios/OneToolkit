#include "pch.h"
#include "AttachedProperties.h"

using namespace Platform;
using namespace AppFramework;
using namespace AppFramework::Controls;
using namespace Windows::Foundation;
using namespace OneToolkit::UI::Xaml;

DependencyProperty^ AttachedProperties::m_SpacingProperty = DependencyProperty::RegisterAttached("Spacing", double::typeid, AttachedProperties::typeid, ref new PropertyMetadata(nullptr, ref new PropertyChangedCallback(&AttachedProperties::DependencyPropertyChanged)));

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
	return m_SpacingProperty;
}

double AttachedProperties::GetSpacing(ListViewBase^ listView)
{
	if (auto propertyValue = dynamic_cast<IBox<double>^>(listView->GetValue(m_SpacingProperty))) return propertyValue->Value;
	else return 0;
}

void AttachedProperties::SetSpacing(ListViewBase^ listView, double value)
{
	listView->SetValue(m_SpacingProperty, value);
}

void AttachedProperties::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	if (e->Property == m_SpacingProperty)
	{
		auto listView = dynamic_cast<ListViewBase^>(sender);
		auto oldSpacing = static_cast<double>(e->OldValue);
		auto newSpacing = static_cast<double>(e->NewValue);

		// Set a margin with the spacing value for each item in the view.
		SetSpacingForItems(listView, oldSpacing, newSpacing);

		// Set a negative margin on the view to avoid extra space after the last row/column.
		auto margin = listView->Margin;
		margin.Right -= oldSpacing;
		margin.Bottom -= oldSpacing;
		margin.Right += -newSpacing;
		margin.Bottom += -newSpacing;
		listView->Margin = margin;

		listView->ChoosingItemContainer += ref new TypedEventHandler<AppFramework::Controls::ListViewBase^, AppFramework::Controls::ChoosingItemContainerEventArgs^>(&AttachedProperties::OnChoosingItemContainer);
	}
}

void AttachedProperties::OnChoosingItemContainer(AppFramework::Controls::ListViewBase^ sender, AppFramework::Controls::ChoosingItemContainerEventArgs^ args)
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