#include "pch.h"
#include "ContentDialogHelper.h"
#include "Controls.ContentDialogHelper.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::OneToolkit::UI::WinUI::Controls::implementation
{
    IAsyncOperation<bool> ContentDialogHelper::TryShowAsync(ContentDialog dialog, ContentDialogPlacement placement, ContentDialogDisplayCompletionAsyncHandler completionHandler, IViewServiceProvider viewServiceProvider)
    {
        if (!viewServiceProvider.IsDialogShown())
        {
            viewServiceProvider.IsDialogShown(true);
            auto result = co_await dialog.ShowAsync();
            co_await completionHandler(dialog, result);
            viewServiceProvider.IsDialogShown(false);
            co_return true;
        }

        co_return false;
    }
}