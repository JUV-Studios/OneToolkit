#pragma once
#include "Storage.BufferHelper.g.h"

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct BufferHelper : BufferHelperT<BufferHelper>
        {
            BufferHelper() = delete;
            static uint8_t GetAt(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index);
            static void SetAt(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index, uint8_t value);
            static bool IsValidIndex(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index);
        };
    }

    namespace factory_implementation
    {
        struct BufferHelper : BufferHelperT<BufferHelper, implementation::BufferHelper>
        {
        };
    }
}