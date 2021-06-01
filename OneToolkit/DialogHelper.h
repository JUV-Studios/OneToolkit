#pragma once
#include "UI.DialogHelper.g.h"

namespace winrt::OneToolkit::UI
{
    namespace implementation
    {
        struct DialogHelper : DialogHelperT<DialogHelper>
        {
            DialogHelper() = delete;
            static bool IsDialogShown() noexcept;
            static void IsDialogShown(bool value) noexcept;
            static Windows::Foundation::IAsyncOperation<bool> TryShowDialogAsync(Windows::UI::Xaml::Controls::ContentDialog dialog, Windows::UI::Xaml::Controls::ContentDialogPlacement placement,
                DialogDisplayCompletionAsyncHandler completionHandler) noexcept;
        };
    }

    namespace factory_implementation
    {
        struct DialogHelper : DialogHelperT<DialogHelper, implementation::DialogHelper>
        {
        };
    }
}