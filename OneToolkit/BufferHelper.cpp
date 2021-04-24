#include "pch.h"
#include "BufferHelper.h"
#include "Storage.BufferHelper.g.cpp"

using namespace winrt;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

namespace winrt::OneToolkit::Storage::implementation
{
    IBuffer BufferHelper::EmptyBuffer()
    {
        static slim_mutex lock;
        const slim_lock_guard guard { lock };
        static IBuffer emptyBuffer;
        if (emptyBuffer == nullptr) emptyBuffer = CryptographicBuffer::CreateFromByteArray({ 0 });
        return emptyBuffer;
    }

    IBuffer BufferHelper::Concat(array_view<IBuffer const> buffers)
    {
        uint32_t totalBytes = 0;
        std::map<uint32_t, uint32_t> indexDetails;
        for (uint32_t index = 0; index < buffers.size(); ++index)
        {
            auto bufferSize = buffers[index].Length();
            indexDetails[index] = bufferSize;
            totalBytes += bufferSize;
        }

        Buffer buffer { totalBytes };
        uint32_t writtenBytes = 0;
        for (uint32_t index = 0; index < buffers.size(); ++index)
        {
            auto bufferSize = indexDetails[index];
            memcpy(buffer.data() + writtenBytes, buffers[index].data(), bufferSize);
            writtenBytes = bufferSize;
        }

        return buffer;
    }

    uint8_t BufferHelper::GetAt(IBuffer const& buffer, uint32_t index)
    {
        if (index >= buffer.Capacity()) throw hresult_out_of_bounds();
        return buffer.data()[index];
    }

    void BufferHelper::SetAt(IBuffer const& buffer, uint32_t index, uint8_t val)
    {
        if (index >= buffer.Capacity()) throw hresult_out_of_bounds();
        buffer.data()[index] = val;
    }

    bool BufferHelper::IsDataEqual(IBuffer const& first, IBuffer const& second) noexcept
    {
        if (first == second) return true;
        else if (first.Length() == second.Length())
        {
            for (uint32_t index = 0; index < first.Length(); ++index)
            {
                if (first.data()[index] != second.data()[index]) return false;
            }

            return true;
        }

        return false;
    }
}