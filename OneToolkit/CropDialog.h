#pragma once
#include "Media.Imaging.CropDialog.g.h"

namespace winrt::OneToolkit::Media::Imaging
{
    namespace implementation
    {
        struct CropDialog : CropDialogT<CropDialog>
        {
            Windows::Foundation::IAsyncOperation<bool> CropAsync(Windows::Storage::StorageFile origin, Windows::Storage::StorageFile destination) const;
            DeclareAutoProperty(bool, IsEllipticalCrop, false);
            DeclareAutoProperty(Windows::Foundation::Size, CropSize, Windows::Foundation::Size(500, 500));
            static Windows::Foundation::Uri PickerUri();
            static Windows::Foundation::IAsyncOperation<bool> IsSupportedAsync();
        };
    }

    namespace factory_implementation
    {
        struct CropDialog : CropDialogT<CropDialog, implementation::CropDialog>
        {
        };
    }
}