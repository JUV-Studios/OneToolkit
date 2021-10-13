#include "pch.h"
#include "BufferHelper.h"
#include "Storage.BufferHelper.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

namespace winrt::OneToolkit::Storage::implementation
{
	__interface __declspec(uuid("5b0d3235-4dba-4d44-865e-8f1d0e4fd04d")) IMemoryBufferByteAccess : ::IUnknown
	{
		int __stdcall GetBuffer(uint8** value, uint32* capacity);
	};

	uint8 BufferHelper::GetAt(IBuffer const& buffer, uint32 index)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		return buffer.data()[index];
	}

	uint8 BufferHelper::GetAt(IMemoryBufferReference const& reference, uint32 index)
	{
		if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
		else
		{
			uint8* data;
			check_hresult(reference.as<IMemoryBufferByteAccess>()->GetBuffer(&data, nullptr));
			return data[index];
		}
	}

	void BufferHelper::SetAt(IBuffer const& buffer, uint32 index, uint8 value)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		buffer.data()[index] = value;
	}

	void BufferHelper::SetAt(IMemoryBufferReference const& reference, uint32 index, uint8 value)
	{
		if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
		else
		{
			uint8* data;
			check_hresult(reference.as<IMemoryBufferByteAccess>()->GetBuffer(&data, nullptr));
			data[index] = value;
		}
	}

	bool BufferHelper::IsIndexValid(IBuffer const& buffer, uint32 index)
	{
		return index < buffer.Capacity();
	}

	bool BufferHelper::IsIndexValid(IMemoryBufferReference const& reference, uint32 index)
	{
		return index < reference.Capacity();
	}
}