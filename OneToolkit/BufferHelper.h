#pragma once
#include "BufferHelper.g.h"

namespace winrt::OneToolkit::implementation
{
    struct BufferHelper : BufferHelperT<BufferHelper>
    {
        BufferHelper() = delete;
        static Windows::Storage::Streams::IBuffer EmptyBuffer();
        static uint8_t GetDataAt(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index);
        static void SetDataAt(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index, uint8_t val);
        static bool IsDataEqual(Windows::Storage::Streams::IBuffer const& first, Windows::Storage::Streams::IBuffer const& second) noexcept;
    };
}
namespace winrt::OneToolkit::factory_implementation
{
    struct BufferHelper : BufferHelperT<BufferHelper, implementation::BufferHelper>
    {
    };
}
