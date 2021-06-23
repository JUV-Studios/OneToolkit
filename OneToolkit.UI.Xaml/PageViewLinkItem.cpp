#include "pch.h"
#include "PageView.xaml.h"
#include "PageViewLinkItem.h"

using namespace Platform;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace OneToolkit::UI::Xaml::Controls;

PageViewLinkItem::PageViewLinkItem()
{
	SelectsOnInvoked = false;
	m_LauncherOptions->TreatAsUntrusted = true;
}

LauncherOptions^ PageViewLinkItem::LauncherOptions::get()
{
	return m_LauncherOptions;
}

void PageViewLinkItem::Invoke(PageView^ sender)
{
	InvokeAsync(sender);
}

concurrency::task<void> PageViewLinkItem::InvokeAsync(PageView^ sender)
{
	if (!sender->ViewServiceProvider->IsDialogShown)
	{
		sender->ViewServiceProvider->IsDialogShown = true;
		co_await Launcher::LaunchUriAsync(NavigateUri, m_LauncherOptions);
		sender->ViewServiceProvider->IsDialogShown = false;
	}
}