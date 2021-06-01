#include "pch.h"
#include "BufferHelper.h"
#include "Storage.BufferHelper.g.cpp"

using namespace winrt;
using namespace Windows::Storage::Streams;

namespace winrt::OneToolkit::Storage::implementation
{
	uint8_t BufferHelper::GetAt(IBuffer const& buffer, uint32_t index)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		return buffer.data()[index];
	}

	void BufferHelper::SetAt(IBuffer const& buffer, uint32_t index, uint8_t value)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		buffer.data()[index] = value;
	}

	bool BufferHelper::IsIndexValid(IBuffer const& buffer, uint32_t index)
	{
		return index < buffer.Length();
	}
}