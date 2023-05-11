module;

#include "OneToolkit.h"
#include <span>
#include <memory>
#include <format>
#include <cstddef>
#include <concepts>
#include <filesystem>
#include <Unknwn.h>
#include <Windows.h>
#include <WinString.h>
#include <winrt/Windows.Storage.h>
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
		struct StringOperations
		{
			StringOperations() = delete;

			static auto Trim(hstring const& text, hstring const& trimChars)
			{
				HSTRING output;
				check_hresult(WindowsTrimStringStart(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimChars)), &output));
				check_hresult(WindowsTrimStringEnd(static_cast<HSTRING>(get_abi(output)), static_cast<HSTRING>(get_abi(trimChars)), &output));
				return hstring(output, take_ownership_from_abi);
			}

			static auto TrimStart(hstring const& text, hstring const& trimChars)
			{
				HSTRING output;
				check_hresult(WindowsTrimStringStart(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimChars)), &output));
				return hstring(output, take_ownership_from_abi);
			}

			static auto TrimEnd(hstring const& text, hstring const& trimChars)
			{
				HSTRING output;
				check_hresult(WindowsTrimStringEnd(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimChars)), &output));
				return hstring(output, take_ownership_from_abi);
			}

			static auto Replace(hstring const& text, hstring const& replaced, hstring const& replacedWith)
			{
				HSTRING output;
				check_hresult(WindowsReplaceString(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(replaced)), static_cast<HSTRING>(get_abi(replacedWith)), &output));
				return hstring(output, take_ownership_from_abi);
			}

			static auto Substring(hstring const& text, uint32 startIndex)
			{
				HSTRING output;
				check_hresult(WindowsSubstring(static_cast<HSTRING>(get_abi(text)), startIndex, &output));
				return hstring(output, take_ownership_from_abi);
			}

			static auto Substring(hstring const& text, uint32 startIndex, uint32 length)
			{
				HSTRING output;
				check_hresult(WindowsSubstringWithSpecifiedLength(static_cast<HSTRING>(get_abi(text)), startIndex, length, &output));
				return hstring(output, take_ownership_from_abi);
			}

			static auto Split(hstring const& text, hstring const& delimiter)
			{
				std::vector<hstring> values;
				std::wstring textCopy{ text };
				wchar_t* state = nullptr;
				auto token = wcstok_s(textCopy.data(), delimiter.data(), &state);
				while (token != nullptr)
				{
					values.emplace_back(token);
					token = wcstok_s(nullptr, delimiter.data(), &state);
				}

				return values;
			}
		};

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
				if (m_IsDisposed) throw hresult_error(RO_E_CLOSED, message);
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

	namespace Storage
	{
		[flags]
		enum class HandleOptions
		{
			None = 0,
			OpenRequiringOplock = 0x40000,
			DeleteOnClose = 0x4000000,
			SequentialScan = 0x8000000,
			RandomAccess = 0x10000000,
			NoBuffering = 0x20000000,
			Overlapped = 0x40000000,
			WriteThrough = 0x80000000
		};

		[flags]
		enum class HandleAccessOptions
		{
			None = 0,
			ReadAttributes = 0x80,
			Read = 0x120089,
			Write = 0x120116,
			Delete = 0x10000
		};

		[flags]
		enum class HandleSharingOptions
		{
			ShareNone = 0,
			ShareRead = 0x1,
			ShareWrite = 0x2,
			ShareDelete = 0x4
		};
		
		enum class HandleCreationOptions
		{
			CreateNew = 0x1,
			CreateAlways = 0x2,
			OpenExisting = 0x3,
			OpenAlways = 0x4,
			TruncateExisting = 0x5
		};

		MIDL_INTERFACE("826ABE3D-3ACD-47D3-84F2-88AAEDCF6304") IOplockBreakingHandler : ::IUnknown
		{
			STDMETHOD(OplockBreaking)() = 0;
		};

		auto CreateFileHandle(IStorageItem const& storageItem, HandleAccessOptions accessOptions, HandleSharingOptions sharingOptions, HandleOptions options, IOplockBreakingHandler* oplockBreakingHandler = nullptr)
		{
			MIDL_INTERFACE("5CA296B2-2C25-4D22-B785-B885C8201E6A") IStorageItemHandleAccess : ::IUnknown
			{
				STDMETHOD(Create)(HandleAccessOptions accessOptions, HandleSharingOptions sharingOptions, HandleOptions options, IOplockBreakingHandler* oplockBreakingHandler, HANDLE* interopHandle) = 0;
			};

			HANDLE result;
			check_hresult(storageItem.as<IStorageItemHandleAccess>()->Create(accessOptions, sharingOptions, options, oplockBreakingHandler, &result));
			return file_handle(result);
		}

		auto CreateFileHandle(IStorageFolder const& storageFile, hstring const& fileName, HandleCreationOptions creationOptions, HandleAccessOptions accessOptions, HandleSharingOptions sharingOptions, HandleOptions options, IOplockBreakingHandler oplockBreakingHandler = nullptr)
		{
			MIDL_INTERFACE("DF19938F-5462-48A0-BE65-D2A3271A08D6") IStorageFolderHandleAccess : ::IUnknown
			{
				STDMETHOD(Create)(wchar_t const* fileName, HandleCreationOptions creationOptions, HandleAccessOptions accessOptions, HandleSharingOptions sharingOptions, HandleOptions options, IOplockBreakingHandler* oplockBreakingHandler, HANDLE* interopHandle) = 0;
			};

			HANDLE result;
			check_hresult(storageFile.as<IStorageFolderHandleAccess>()->Create(fileName.data(), creationOptions, accessOptions, sharingOptions, options, oplockBreakingHandler, &result));
			return file_handle(result);
		}
	}
}