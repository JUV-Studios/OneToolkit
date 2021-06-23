#include "pch.h"
#include "SharedFile.h"
#include "Storage.SharedFile.g.cpp"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::DataTransfer;

namespace winrt::OneToolkit::Storage::implementation
{
	SharedFile::SharedFile(StorageFile const& file)
	{
		m_Token = SharedStorageAccessManager::AddFile(file);
	}

	hstring SharedFile::Token() const noexcept
	{
		return m_Token;
	}

	IAsyncOperation<StorageFile> SharedFile::GetFileAsync() const
	{
		return SharedStorageAccessManager::RedeemTokenForFileAsync(m_Token);
	}

	void SharedFile::Dispose() const
	{
		SharedStorageAccessManager::RemoveFile(m_Token);
	}
}