#include "pch.h"
#include "PageViewLinkItem.h"

using namespace Windows::System;
using namespace OneToolkit::UI::Controls;

PageViewLinkItem::PageViewLinkItem()
{
	SelectsOnInvoked = false;
	Options = ref new LauncherOptions();
	Options->TreatAsUntrusted = true;
}

void PageViewLinkItem::Raise(bool result)
{
	AfterLinkLaunched(result);
}