#include "pch.h"
#include "BufferHelper.h"
#include "BufferHelper.g.cpp"

using namespace winrt;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

namespace winrt::OneToolkit::implementation
{
    IBuffer BufferHelper::EmptyBuffer()
    {
        static IBuffer emptyBuffer;
        if (emptyBuffer == nullptr) emptyBuffer = CryptographicBuffer::CreateFromByteArray({ 0 });
        return emptyBuffer;
    }

    uint8_t BufferHelper::GetDataAt(IBuffer const& buffer, uint32_t index)
    {
        if (index > buffer.Capacity()) throw hresult_out_of_bounds();
        return buffer.data()[index];
    }

    void BufferHelper::SetDataAt(IBuffer const& buffer, uint32_t index, uint8_t val)
    {
        if (index > buffer.Capacity()) throw hresult_out_of_bounds();
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