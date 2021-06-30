#include "pch.h"
#include "PageView.xaml.h"
#include "PageViewLinkItem.h"

using namespace Platform;
using namespace concurrency;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace OneToolkit::ApplicationModel;
using namespace OneToolkit::UI::Xaml::Controls;

PageViewLinkItem::PageViewLinkItem()
{
	SelectsOnInvoked = false;
}

void PageViewLinkItem::Invoke(PageView^ sender)
{
	auto viewService = AppInformation::IsCoreApplication ? ViewService::GetForCurrentView() : ViewService::GetDefault();
	if (!viewService->IsDialogShown)
	{
		viewService->IsDialogShown = true;
		create_task(Launcher::LaunchUriAsync(NavigateUri)).then([viewService](bool)
			{
				viewService->IsDialogShown = false;
			});
	}
}