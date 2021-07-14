#include "pch.h"
#include "BufferHelper.h"
#include "Storage.BufferHelper.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::Storage::Streams;

namespace winrt::OneToolkit::Storage::implementation
{
	uint8 BufferHelper::GetAt(IBuffer const& buffer, uint32 index)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		return buffer.data()[index];
	}

	void BufferHelper::SetAt(IBuffer const& buffer, uint32 index, uint8 value)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		buffer.data()[index] = value;
	}

	bool BufferHelper::IsIndexValid(IBuffer const& buffer, uint32 index)
	{
		return index < buffer.Capacity();
	}
}