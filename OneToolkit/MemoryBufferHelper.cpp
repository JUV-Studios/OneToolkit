#include "pch.h"
#include "MemoryBufferHelper.h"
#include "Storage.MemoryBufferHelper.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::Storage::implementation
{
	__interface __declspec(uuid("5b0d3235-4dba-4d44-865e-8f1d0e4fd04d")) IMemoryBufferByteAccess : ::IUnknown
	{
		HRESULT __stdcall GetBuffer(uint8_t** value, uint32_t* capacity);
	};

	uint8_t MemoryBufferHelper::GetAt(IMemoryBufferReference const& reference, uint32_t index)
	{
		if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
		else
		{
			uint8_t* data;
			check_hresult(reference.as<IMemoryBufferByteAccess>()->GetBuffer(&data, nullptr));
			return data[index];
		}
	}

	void MemoryBufferHelper::SetAt(IMemoryBufferReference const& reference, uint32_t index, uint8_t value)
	{
		if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
		else
		{
			uint8_t* data;
			check_hresult(reference.as<IMemoryBufferByteAccess>()->GetBuffer(&data, nullptr));
			data[index] = value;
		}
	}

	bool MemoryBufferHelper::IsIndexValid(IMemoryBufferReference const& reference, uint32_t index)
	{
		return index < reference.Capacity();
	}
}