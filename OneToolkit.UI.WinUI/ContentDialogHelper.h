#pragma once
#include "Controls.ContentDialogHelper.g.h"

namespace winrt::OneToolkit::UI::WinUI::Controls
{
    namespace implementation
    {
        struct ContentDialogHelper
        {
            ContentDialogHelper() = delete;
            static Windows::Foundation::IAsyncOperation<bool> TryShowAsync(Microsoft::UI::Xaml::Controls::ContentDialog dialog, Microsoft::UI::Xaml::Controls::ContentDialogPlacement placement, 
                ContentDialogDisplayCompletionAsyncHandler completionHandler, ViewService viewService);
        };
    }
    
    namespace factory_implementation
    {
        struct ContentDialogHelper : ContentDialogHelperT<ContentDialogHelper, implementation::ContentDialogHelper>
        {
        };
    }
}