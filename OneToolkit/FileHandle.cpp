#include "pch.h"
#include "FileHandle.h"
#include "Storage.FileHandle.g.cpp"
#include <WindowsStorageCOM.h>

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace OneToolkit::Lifecycle;

namespace winrt::OneToolkit::Storage::implementation
{
	constexpr auto AsAccessOption(FileAccessMode accessMode) noexcept
	{
		return accessMode == FileAccessMode::Read ? HAO_READ | HAO_READ_ATTRIBUTES : HAO_READ | HAO_READ_ATTRIBUTES | HAO_WRITE | HAO_DELETE;
	}

	FileHandle::FileHandle(StorageFile const& file, FileAccessMode accessMode, FileSharingMode sharingMode) : m_StorageItem(file), m_ItemName(file.Name()), m_AccessMode(accessMode), m_SharingMode(sharingMode)
	{
		check_hresult(file.as<IStorageItemHandleAccess>()->Create(AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr, m_FileHandle.put()));
	}

	FileHandle::FileHandle(StorageFolder const& folder, hstring const& fileName, FileAccessMode accessMode, FileSharingMode sharingMode) : m_StorageItem(folder), m_ItemName(fileName), m_AccessMode(accessMode), m_SharingMode(sharingMode)
	{
		check_hresult(folder.as<IStorageFolderHandleAccess>()->Create(fileName.data(), HCO_CREATE_NEW, AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr,
			m_FileHandle.put()));
	}

	uint64 FileHandle::FileSize() const
	{
		LARGE_INTEGER result;
		check_bool(GetFileSizeEx(m_FileHandle.get(), &result));
		return result.QuadPart;
	}

	StorageItemId FileHandle::Id() const noexcept
	{
		return { as_value<uint64>(m_FileHandle.get()) };
	}

	IAsyncOperation<StorageFile> FileHandle::GetStorageFileAsync() const
	{
		if (auto folder = m_StorageItem.try_as<StorageFolder>()) return folder.GetFileAsync(m_ItemName);
		else return make<AsyncOperationWrapper<StorageFile>>(m_StorageItem.as<StorageFile>());
	}

	void FileHandle::Delete()
	{
		FILE_DISPOSITION_INFO info { true };
		check_bool(SetFileInformationByHandle(m_FileHandle.get(), FileDispositionInfo, &info, sizeof(FILE_DISPOSITION_INFO)));
	}
}