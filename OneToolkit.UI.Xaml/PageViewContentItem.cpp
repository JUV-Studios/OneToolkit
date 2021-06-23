#include "pch.h"
#include "PageView.xaml.h"
#include "PageViewContentItem.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;
using namespace OneToolkit::UI::Xaml::Controls;

void PageViewContentItem::Invoke(PageView^ sender)
{
	sender->CurrentContent = SelectionContent;
}