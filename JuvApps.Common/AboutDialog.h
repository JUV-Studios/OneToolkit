#pragma once
#include "AboutDialog.g.h"

namespace winrt::JuvApps::Common
{
    namespace implementation
    {
        struct AboutDialog : AboutDialogT<AboutDialog>
        {
            void ContentDialog_Loaded(IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& e);
        };
    }

    namespace factory_implementation
    {
        struct AboutDialog : AboutDialogT<AboutDialog, implementation::AboutDialog>
        {
        };
    }
}