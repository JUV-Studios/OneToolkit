#pragma once
#include "Storage.SharedFile.g.h"

namespace winrt::OneToolkit::Storage
{
	namespace implementation
	{
		struct SharedFile : SharedFileT<SharedFile>, Lifecycle::Disposable<SharedFile>
		{
		public:
			SharedFile(Windows::Storage::StorageFile const& file);
			hstring Token() const noexcept;
			Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFile> GetFileAsync() const;
			void Dispose() const;
		private:
			hstring m_Token;
		};
	}

	namespace factory_implementation
	{
		struct SharedFile : SharedFileT<SharedFile, implementation::SharedFile>
		{
		};
	}
}