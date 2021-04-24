#pragma once
#include "Storage.BufferHelper.g.h"

namespace winrt::OneToolkit::Storage::implementation
{
    struct BufferHelper : BufferHelperT<BufferHelper>
    {
        BufferHelper() = delete;
        static Windows::Storage::Streams::IBuffer EmptyBuffer();
        static Windows::Storage::Streams::IBuffer Concat(array_view<winrt::Windows::Storage::Streams::IBuffer const> buffers);
        static uint8_t GetAt(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index);
        static void SetAt(Windows::Storage::Streams::IBuffer const& buffer, uint32_t index, uint8_t val);
        static bool IsDataEqual(Windows::Storage::Streams::IBuffer const& first, Windows::Storage::Streams::IBuffer const& second) noexcept;
    };
}
namespace winrt::OneToolkit::Storage::factory_implementation
{
    struct BufferHelper : BufferHelperT<BufferHelper, implementation::BufferHelper>
    {
    };
}
