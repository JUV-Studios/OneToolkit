#pragma once
#include "Storage.BufferHelper.g.h"

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct BufferHelper : BufferHelperT<BufferHelper>
        {
            BufferHelper() = delete;
            static juv::uint8 GetAt(Windows::Storage::Streams::IBuffer const& buffer, juv::uint32 index);
            static void SetAt(Windows::Storage::Streams::IBuffer const& buffer, juv::uint32 index, juv::uint8 value);
            static bool IsIndexValid(Windows::Storage::Streams::IBuffer const& buffer, juv::uint32 index);
        };
    }

    namespace factory_implementation
    {
        struct BufferHelper : BufferHelperT<BufferHelper, implementation::BufferHelper>
        {
        };
    }
}