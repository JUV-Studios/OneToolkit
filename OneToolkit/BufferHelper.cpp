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
		static slim_mutex emptyBuffer_Mutex;
		const winrt::slim_lock_guard lock { emptyBuffer_Mutex };
		static IBuffer emptyBuffer;
		if (!emptyBuffer) emptyBuffer = CryptographicBuffer::CreateFromByteArray({ 0 });
		return emptyBuffer;
	}

	uint8_t BufferHelper::GetAt(IBuffer const& buffer, uint32_t index)
	{
		if (index >= buffer.Capacity()) throw hresult_out_of_bounds();
		return buffer.data()[index];
	}

	void BufferHelper::SetAt(IBuffer const& buffer, uint32_t index, uint8_t value)
	{
		if (index >= buffer.Capacity()) throw hresult_out_of_bounds();
		buffer.data()[index] = value;
	}
}