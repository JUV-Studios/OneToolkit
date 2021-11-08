#include "pch.h"
#include "StorageItemHandle.h"
#include "Storage.FileHandle.g.cpp"
#include "Storage.FolderHandle.g.cpp"
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

	FileHandle::FileHandle(StorageFile const& file, FileAccessMode accessMode, FileSharingMode sharingMode) : m_StorageItem(file), ItemName(file.Name()), AccessMode(accessMode), SharingMode(sharingMode)
	{
		check_hresult(file.as<IStorageItemHandleAccess>()->Create(AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr, m_FileHandle.put()));
	}

	FileHandle::FileHandle(StorageFolder const& folder, hstring const& fileName, FileAccessMode accessMode, FileSharingMode sharingMode) : m_StorageItem(folder), ItemName(fileName), AccessMode(accessMode), SharingMode(sharingMode)
	{
		check_hresult(folder.as<IStorageFolderHandleAccess>()->Create(fileName.data(), HCO_CREATE_NEW, AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr,
			m_FileHandle.put()));
	}

	hstring FileHandle::DisplayName() const
	{
		if (auto file = m_StorageItem.try_as<StorageFile>()) return file.DisplayName();
		else return std::filesystem::path(file.DisplayName().data()).replace_extension(L"").c_str();
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

	void FileHandle::Delete()
	{
		FILE_DISPOSITION_INFO info{ true };
		check_bool(SetFileInformationByHandle(m_FileHandle.get(), FileDispositionInfo, &info, sizeof(FILE_DISPOSITION_INFO)));
	}

	IAsyncOperation<StorageFile> FileHandle::GetStorageFileAsync() const
	{
		if (auto folder = m_StorageItem.try_as<StorageFolder>()) return folder.GetFileAsync(ItemName());
		else return make<AsyncOperationWithResult<StorageFile>>(m_StorageItem.as<StorageFile>());
	}

	FolderHandle::FolderHandle(StorageFolder const& folder, FileAccessMode accessMode, FileSharingMode sharingMode) : m_StorageFolder(folder), AccessMode(accessMode), SharingMode(sharingMode)
	{
		check_hresult(folder.as<IStorageItemHandleAccess>()->Create(AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr, m_FolderHandle.put()));
	}

	hstring FolderHandle::ItemName() const
	{
		return m_StorageFolder.Name();
	}

	hstring FolderHandle::DisplayName() const
	{
		return m_StorageFolder.DisplayName();
	}

	StorageItemId FolderHandle::Id() const noexcept
	{
		return { as_value<uint64>(m_FolderHandle.get()) };
	}

	void FolderHandle::Delete()
	{
		FILE_DISPOSITION_INFO info{ true };
		check_bool(SetFileInformationByHandle(m_FolderHandle.get(), FileDispositionInfo, &info, sizeof(FILE_DISPOSITION_INFO)));
	}
	
	IAsyncOperation<StorageFolder> FolderHandle::GetStorageFolderAsync() const
	{
		co_return m_StorageFolder;
	}
}