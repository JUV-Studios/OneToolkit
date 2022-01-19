#include "Storage.SharedFile.g.h"
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::DataTransfer;

namespace winrt::OneToolkit::Storage
{
	namespace implementation
	{
		struct SharedFile : SharedFileT<SharedFile>, Lifecycle::Disposable<SharedFile>
		{
			SharedFile(StorageFile const& file)
			{
				Token(SharedStorageAccessManager::AddFile(file));
			}

			auto_property<hstring> Token;

			IAsyncOperation<StorageFile> GetFileAsync() const
			{
				return SharedStorageAccessManager::RedeemTokenForFileAsync(Token());
			}

			void Dispose() const noexcept
			{
				SharedStorageAccessManager::RemoveFile(Token());
			}
		};
	}

	namespace factory_implementation
	{
		struct SharedFile : SharedFileT<SharedFile, implementation::SharedFile>
		{
		};
	}
}

#include "Storage.SharedFile.g.cpp"