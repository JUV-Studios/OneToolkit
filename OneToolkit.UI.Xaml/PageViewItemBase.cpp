#include "pch.h"
#include "PageViewItemBase.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;
using namespace OneToolkit::UI::Xaml::Controls;

String^ PageViewItemBase::Label::get()
{
	return Content->ToString();
}

void PageViewItemBase::Label::set(String^ value)
{
	Content = value;
}