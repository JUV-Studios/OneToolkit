#include "pch.h"
#include "PageView.xaml.h"
#include "PageViewLinkItem.h"

using namespace Platform;
using namespace concurrency;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace OneToolkit::UI::Xaml::Controls;

PageViewLinkItem::PageViewLinkItem()
{
	SelectsOnInvoked = false;
}

void PageViewLinkItem::Invoke(PageView^ sender)
{
	InvokeAsync();
}

task<void> PageViewLinkItem::InvokeAsync()
{
	auto viewService = ViewService::GetForCurrentView();
	if (!viewService->IsDialogShown)
	{
		viewService->IsDialogShown = true;
		co_await Launcher::LaunchUriAsync(NavigateUri);
		viewService->IsDialogShown = false;
	}
}
