#pragma once
#include "Media.Imaging.CropDialog.g.h"
#include "Media.Imaging.ImageResizer.g.h"

namespace winrt::OneToolkit::Media::Imaging
{
    namespace implementation
    {
        struct CropDialog : CropDialogT<CropDialog>
        {
            Windows::Foundation::IAsyncOperation<bool> CropAsync(Windows::Storage::StorageFile origin, Windows::Storage::StorageFile destination) const;
            juv::auto_property<bool> IsEllipticalCrop;
            juv::auto_property<Windows::Foundation::Size> CropSize{ {500, 500} };
            static Windows::Foundation::Uri PickerUri();
            static Windows::Foundation::IAsyncOperation<bool> IsSupportedAsync();
        };

        struct ImageResizer : ImageResizerT<ImageResizer>
        {
            ImageResizer() = delete;
            static Windows::Foundation::IAsyncAction RescaleAsync(Windows::Foundation::Size desiredSize, Windows::Graphics::Imaging::BitmapInterpolationMode interpolationMode, Windows::Storage::Streams::IRandomAccessStream const& inputStream,
                Windows::Storage::Streams::IRandomAccessStream const& outputStream);
        };
    }

    namespace factory_implementation
    {
        struct CropDialog : CropDialogT<CropDialog, implementation::CropDialog>
        {
        };

        struct ImageResizer : ImageResizerT<ImageResizer, implementation::ImageResizer>
        {
        };
    }
}