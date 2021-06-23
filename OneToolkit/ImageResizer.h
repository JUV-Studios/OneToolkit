#pragma once
#include "Imaging.ImageResizer.g.h"

namespace winrt::OneToolkit::Imaging
{
    namespace implementation
    {
        struct ImageResizer : ImageResizerT<ImageResizer>
        {
            ImageResizer() = delete;
            static Windows::Foundation::IAsyncAction RescaleAsync(Windows::Foundation::Size desiredSize, Windows::Graphics::Imaging::BitmapInterpolationMode interpolationMode, Windows::Storage::Streams::IRandomAccessStream const& inputStream,
                Windows::Storage::Streams::IRandomAccessStream const& outputStream);
        };
    }

    namespace factory_implementation
    {
        struct ImageResizer : ImageResizerT<ImageResizer, implementation::ImageResizer>
        {
        };
    }
}