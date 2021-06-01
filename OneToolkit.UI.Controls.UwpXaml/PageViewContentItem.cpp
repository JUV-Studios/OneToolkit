#include "pch.h"
#include "PageView.xaml.h"
#include "PageViewContentItem.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;
using namespace OneToolkit::UI::Controls;

String^ PageViewContentItem::Label::get()
{
	return Content->ToString();
}

void PageViewContentItem::Label::set(String^ value)
{
	if (Content != value)
	{
		Content = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs("Label"));
	}
}

void PageViewContentItem::Invoke(PageView^ sender)
{
	sender->CurrentContent = DisplayContent;
}