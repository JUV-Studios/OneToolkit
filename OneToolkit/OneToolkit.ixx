module;

#include "OneToolkit.h"
#include <span>
#include <memory>
#include <format>
#include <cstddef>
#include <concepts>
#include <filesystem>
#include <Windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/OneToolkit.System.h>
#include <winrt/OneToolkit.UI.Windowing.h>
#include <winrt/OneToolkit.Storage.Logging.h>

export module OneToolkit;

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace OneToolkit::System;

export namespace winrt::OneToolkit
{
	namespace System
	{
		template <typename Derived>
		struct WeakSingleton
		{
		public:
			static auto Instance()
			{
				static slim_mutex accessLock;
				static weak_ref<typename Derived::class_type> ref;
				slim_lock_guard const lockGuard{ accessLock };
				auto instance = ref.get();
				if (!instance)
				{
					instance = make<Derived>();
					ref = instance;
				}

				return instance;
			}
		protected:
			WeakSingleton() = default;
		};

		/// @brief Provides a base class to conveniently implement the IClosable interface.
		template <typename Derived>
		struct Disposable
		{
		public:
			void Close()
			{
				if (!m_IsDisposed)
				{
					static_cast<Derived&>(*this).close();
					m_IsDisposed = true;
				}
			}

			~Disposable()
			{
				Close();
			}
		protected:
			/// @brief Gets a value that indicates whether the object has been closed.
			/// @return true if this object is disposed, false otherwise.
			bool IsDisposed() const noexcept
			{
				return m_IsDisposed;
			}

			/// @brief Throws an exception if the object is already closed.
			void ThrowIfDisposed(hstring const& message = {}) const
			{
				if (m_IsDisposed) throw hresult_object_closed(message);
			}
		private:
			bool m_IsDisposed;
		};

		struct AsyncHelper
		{
		public:
			/// @brief Creates an asynchronous operation that returns a predetermined value.
			template <typename Result>
			auto CreateOperationFromResult(Result const result)
			{
				return make<AsyncOperationWithResult<Result>>(result);
			}
		private:
			template <typename Result>
			struct AsyncOperationWithResult : implements<AsyncOperationWithResult<Result>, IAsyncOperation<Result>>
			{
			public:
				AsyncOperationWithResult(Result result) : m_Result(result) {}

				auto Id() const noexcept
				{
					return as_value<uint32>(this);
				}

				auto ErrorCode() const noexcept
				{
					return hresult(0);
				}

				auto Status() const noexcept
				{
					return AsyncStatus::Completed;
				}

				auto Completed() const noexcept
				{
					return m_Completed;
				}

				void Completed(AsyncOperationCompletedHandler<Result> const& value)
				{
					value(*this, AsyncStatus::Completed);
					m_Completed = value;
				}

				auto GetResults() const noexcept
				{
					return m_Result;
				}

				void Cancel() const noexcept {}

				void Close() const noexcept {}
			private:
				Result m_Result;
				AsyncOperationCompletedHandler<Result> m_Completed;
			};
		};
	}
}