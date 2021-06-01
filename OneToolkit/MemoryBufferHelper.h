#pragma once
#include "Storage.MemoryBufferHelper.g.h"

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct MemoryBufferHelper : MemoryBufferHelperT<MemoryBufferHelper>
        {
            MemoryBufferHelper() = delete;
            static uint8_t GetAt(Windows::Foundation::IMemoryBufferReference const& reference, uint32_t index);
            static void SetAt(Windows::Foundation::IMemoryBufferReference const& reference, uint32_t index, uint8_t value);
            static bool IsIndexValid(Windows::Foundation::IMemoryBufferReference const& reference, uint32_t index);
        };
    }

    namespace factory_implementation
    {
        struct MemoryBufferHelper : MemoryBufferHelperT<MemoryBufferHelper, implementation::MemoryBufferHelper>
        {
        };
    }
}