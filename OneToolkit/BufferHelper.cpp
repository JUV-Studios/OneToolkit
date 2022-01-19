#include "Storage.NullBuffer.g.h"
#include "Storage.BufferHelper.g.h"

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace winrt::impl;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace OneToolkit::Storage;
using namespace OneToolkit::Lifecycle;

namespace winrt::OneToolkit::Storage
{
	namespace implementation
	{
		OneToolkit::Storage::NullBuffer emptyBuffer = nullptr;

		/// <summary>
		/// Represents an empty buffer that can be used to clear data streams.
		/// </summary>
		/// <remarks>An instance of this type can be obtained from the BufferHelper.EmptyBuffer property.</remarks>
		struct NullBuffer : NullBufferT<NullBuffer, IMemoryBufferReference, IBufferByteAccess, IMemoryBufferByteAccess>, Disposable<NullBuffer>
		{
			uint32 Capacity() const noexcept
			{
				return 1;
			}

			uint32 Length() const noexcept
			{
				return 1;
			}

			void Length(uint32) const
			{
				throw hresult_not_implemented();
			}

			event_token Closed(TypedEventHandler<IMemoryBufferReference, IInspectable> const& handler)
			{
				return m_Closed.add(handler);
			}

			void Closed(event_token token) noexcept
			{
				return m_Closed.remove(token);
			}

			IMemoryBufferReference CreateReference() const noexcept
			{
				return *this;
			}

			void Dispose()
			{
				if (!m_IsClosed)
				{
					m_IsClosed = true;
					m_Closed(*this, *this);
				}
			}

			int32 __stdcall Buffer(uint8** value) noexcept
			{
				*value = &m_Data;
				return 0;
			}

			int32 __stdcall GetBuffer(uint8** value, uint32* capacity) noexcept
			{
				*capacity = 1;
				*value = &m_Data;
				return 0;
			}
		private:
			uint8 m_Data = 0;
			bool m_IsClosed = false;
			event<TypedEventHandler<IMemoryBufferReference, IInspectable>> m_Closed;
		};

		struct BufferHelper : factory_implementation::BufferHelperT<BufferHelper, BufferHelper>
		{
			static OneToolkit::Storage::NullBuffer EmptyBuffer() noexcept
			{
				static slim_mutex mutex;
				slim_lock_guard const mutexLock{ mutex };
				if (!emptyBuffer) emptyBuffer = make<NullBuffer>();
				return emptyBuffer;
			}

			static uint8 GetAt(IBuffer const& buffer, uint32 index)
			{
				if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
				return buffer.data()[index];
			}

			static uint8 GetAt(IMemoryBufferReference const& reference, uint32 index)
			{
				if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
				return reference.data()[index];
			}

			static void SetAt(IBuffer const& buffer, uint32 index, uint8 value)
			{
				if (!IsIndexValid(buffer, index)) throw hresult_out_of_bounds();
				buffer.data()[index] = value;
			}

			static void SetAt(IMemoryBufferReference const& reference, uint32 index, uint8 value)
			{
				if (!IsIndexValid(reference, index)) throw hresult_out_of_bounds();
				reference.data()[index] = value;
			}

			static bool IsIndexValid(IBuffer const& buffer, uint32 index)
			{
				return index < buffer.Capacity();
			}

			static bool IsIndexValid(IMemoryBufferReference const& reference, uint32 index)
			{
				return index < reference.Capacity();
			}
		};
	}

	namespace factory_implementation
	{
		using BufferHelper = implementation::BufferHelper;
	}
}

#include "Storage.BufferHelper.g.cpp"