#pragma once
#include "Imaging.CropDialog.g.h"

namespace winrt::OneToolkit::Imaging
{
    namespace implementation
    {
        struct CropDialog : CropDialogT<CropDialog>
        {
            static Windows::Foundation::IAsyncOperation<bool> IsSupportedAsync();
            Windows::Foundation::IAsyncOperation<bool> CropAsync(Windows::Storage::StorageFile input, Windows::Storage::StorageFile destination) const;
            DeclareAutoProperty(bool, IsEllipticalCrop, false);
            DeclareAutoProperty(Windows::Foundation::Size, CropSize, Windows::Foundation::Size(500, 500));
        };
    }

    namespace factory_implementation
    {
        struct CropDialog : CropDialogT<CropDialog, implementation::CropDialog>
        {
        };
    }
}