#pragma once
#include "Storage.StreamHelper.g.h"

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct StreamHelper : StreamHelperT<StreamHelper>
        {
            StreamHelper() = delete;
            static Windows::Foundation::IAsyncAction AppendAsync(Windows::Storage::Streams::IRandomAccessStream const& stream, Windows::Storage::Streams::IBuffer const& buffer);
        };
    }

    namespace factory_implementation
    {
        struct StreamHelper : StreamHelperT<StreamHelper, implementation::StreamHelper>
        {
        };
    }
}