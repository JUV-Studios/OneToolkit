#include "OneToolkit.h"
#include "BufferHelper.h"
#include "Storage.BufferHelper.g.cpp"
#include <winrt/Windows.Security.Cryptography.h>

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;
using namespace OneToolkit::Lifecycle;

namespace winrt::OneToolkit::Storage::implementation
{
	using MemoryBufferClosedEventHandler = TypedEventHandler<IMemoryBufferReference, IInspectable>;

	struct NullBuffer : implements<NullBuffer, IClosable, IBuffer, IMemoryBuffer, IMemoryBufferReference, impl::IBufferByteAccess, impl::IMemoryBufferByteAccess>, Disposable<NullBuffer>
	{
	public:
		uint32 Capacity() const noexcept
		{
			return static_cast<uint32>(m_Array.size());
		}

		uint32 Length() const noexcept
		{
			return static_cast<uint32>(m_Array.size());
		}

		void Length(uint32) const
		{
			throw hresult_not_implemented();
		}

		int32 __stdcall Buffer(uint8** value) noexcept
		{
			*value = m_Array.data();
			return 0;
		}

		int32 __stdcall GetBuffer(uint8** value, uint32* capacity) noexcept
		{
			*value = m_Array.data();
			*capacity = static_cast<uint32>(m_Array.size());
			return 0;
		}

		IMemoryBufferReference CreateReference() const noexcept
		{
			return *this;
		}

		void Dispose()
		{
			m_Closed(*this, nullptr);
		}

		event_token Closed(MemoryBufferClosedEventHandler const& handler)
		{
			return m_Closed.add(handler);
		}

		void Closed(event_token token) noexcept
		{
			return m_Closed.remove(token);
		}
	private:
		std::array<uint8, 1> m_Array { 0 };
		event<MemoryBufferClosedEventHandler> m_Closed;
	};

	IClosable nullBuffer;

	uint8 BufferHelper::GetAt(IBuffer const& buffer, uint32 index)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		return buffer.data()[index];
	}

	uint8 BufferHelper::GetAt(IMemoryBufferReference const& reference, uint32 index)
	{
		if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
		return reference.data()[index];
	}

	void BufferHelper::SetAt(IBuffer const& buffer, uint32 index, uint8 value)
	{
		if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
		buffer.data()[index] = value;
	}

	void BufferHelper::SetAt(IMemoryBufferReference const& reference, uint32 index, uint8 value)
	{
		if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
		reference.data()[index] = value;
	}

	bool BufferHelper::IsIndexValid(IBuffer const& buffer, uint32 index)
	{
		return index < buffer.Capacity();
	}

	bool BufferHelper::IsIndexValid(IMemoryBufferReference const& reference, uint32 index)
	{
		return index < reference.Capacity();
	}

	IBuffer BufferHelper::EmptyBuffer()
	{
		if (!nullBuffer) nullBuffer = make<NullBuffer>();
		return nullBuffer.as<IBuffer>();
	}

	IMemoryBuffer BufferHelper::EmptyMemoryBuffer()
	{
		if (!nullBuffer) nullBuffer = make<NullBuffer>();
		return nullBuffer.as<IMemoryBuffer>();
	}
}