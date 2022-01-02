#include "OneToolkit.h"
#include "StorageItemHandle.h"
#include "Storage.FileHandle.g.cpp"
#include "Storage.FolderHandle.g.cpp"
#include <filesystem>
#include <winrt/Windows.Foundation.h>

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace OneToolkit::Lifecycle;

namespace winrt::OneToolkit::Storage::implementation
{
	enum class HandleOptions : uint32
	{
		HO_NONE = 0,
		HO_OPEN_REQUIRING_OPLOCK = 0x40000,
		HO_DELETE_ON_CLOSE = 0x4000000,
		HO_SEQUENTIAL_SCAN = 0x8000000,
		HO_RANDOM_ACCESS = 0x10000000,
		HO_NO_BUFFERING = 0x20000000,
		HO_OVERLAPPED = 0x40000000,
		HO_WRITE_THROUGH = 0x80000000
	};

	enum class HandleAccessOptions
	{
		HAO_NONE = 0,
		HAO_READ_ATTRIBUTES = 0x80,
		HAO_READ = 0x120089,
		HAO_WRITE = 0x120116,
		HAO_DELETE = 0x10000
	};

	DEFINE_ENUM_FLAG_OPERATORS(HandleAccessOptions);

	enum class HandleCreationOptions
	{
		HCO_CREATE_NEW = 0x1,
		HCO_CREATE_ALWAYS = 0x2,
		HCO_OPEN_EXISTING = 0x3,
		HCO_OPEN_ALWAYS = 0x4,
		HCO_TRUNCATE_EXISTING = 0x5
	};

	__interface __declspec(uuid("5CA296B2-2C25-4D22-B785-B885C8201E6A")) IStorageItemHandleAccess : ::IUnknown
	{
		int __stdcall Create(HandleAccessOptions accessOptions, FileSharingMode sharingOptions, HandleOptions options, void* oplockBreakingHandler, HANDLE* interopHandle) noexcept;
	};

	__interface __declspec(uuid("DF19938F-5462-48A0-BE65-D2A3271A08D6")) IStorageFolderHandleAccess : ::IUnknown
	{
		int __stdcall Create(wchar const* fileName, HandleCreationOptions creationOptions, HandleAccessOptions accessOptions, FileSharingMode sharingOptions, HandleOptions options, void* oplockBreakingHandler, HANDLE* interopHandle) noexcept;
	};

	auto AsAccessOption(FileAccessMode accessMode) noexcept
	{
		return accessMode == FileAccessMode::Read ? HandleAccessOptions::HAO_READ | HandleAccessOptions::HAO_READ_ATTRIBUTES : HandleAccessOptions::HAO_READ | HandleAccessOptions::HAO_READ_ATTRIBUTES | HandleAccessOptions::HAO_WRITE | HandleAccessOptions::HAO_DELETE;
	}

	StorageItemHandle::StorageItemHandle(IStorageItem const& storageItem, FileAccessMode accessMode, FileSharingMode sharingMode) : m_StorageItem(storageItem), AccessMode(accessMode), SharingMode(sharingMode) {}

	FileHandle::FileHandle(StorageFile const& file, FileAccessMode accessMode, FileSharingMode sharingMode) : StorageItemHandle(file, accessMode, sharingMode), ItemName(file.Name())
	{
		check_hresult(file.as<IStorageItemHandleAccess>()->Create(AsAccessOption(accessMode), sharingMode, HandleOptions::HO_NONE, nullptr, m_ItemHandle.put()));
	}

	FileHandle::FileHandle(StorageFolder const& folder, hstring const& fileName, FileAccessMode accessMode, FileSharingMode sharingMode) : StorageItemHandle(folder, accessMode, sharingMode), ItemName(fileName)
	{
		check_hresult(folder.as<IStorageFolderHandleAccess>()->Create(fileName.data(), HandleCreationOptions::HCO_CREATE_NEW, AsAccessOption(accessMode), sharingMode, HandleOptions::HO_NONE, nullptr,
			m_ItemHandle.put()));
	}

	FolderHandle::FolderHandle(StorageFolder const& folder, FileAccessMode accessMode, FileSharingMode sharingMode) : StorageItemHandle(folder, accessMode, sharingMode)
	{
		check_hresult(folder.as<IStorageItemHandleAccess>()->Create(AsAccessOption(accessMode), sharingMode, HandleOptions::HO_NONE, nullptr, m_ItemHandle.put()));
	}

	StorageItemId StorageItemHandle::Id() const noexcept
	{
		return { as_value<uint64>(m_ItemHandle.get()) };
	}

	hstring FileHandle::DisplayName() const
	{
		if (auto file = m_StorageItem.try_as<StorageFile>()) return file.DisplayName();
		else return std::filesystem::path(file.DisplayName().data()).replace_extension(L"").c_str();
	}

	hstring FolderHandle::ItemName() const
	{
		return m_StorageItem.Name();
	}

	hstring FolderHandle::DisplayName() const
	{
		return m_StorageItem.as<StorageFolder>().DisplayName();
	}

	uint64 FileHandle::FileSize() const
	{
		LARGE_INTEGER result;
		check_bool(GetFileSizeEx(m_ItemHandle.get(), &result));
		return result.QuadPart;
	}

	void StorageItemHandle::Delete()
	{
		FILE_DISPOSITION_INFO info{ true };
		check_bool(SetFileInformationByHandle(m_ItemHandle.get(), FileDispositionInfo, &info, sizeof(FILE_DISPOSITION_INFO)));
	}

	IAsyncOperation<StorageFile> FileHandle::GetStorageFileAsync() const
	{
		if (auto folder = m_StorageItem.try_as<StorageFolder>()) return folder.GetFileAsync(ItemName());
		else return make<AsyncOperationWithResult<StorageFile>>(m_StorageItem.as<StorageFile>());
	}

	IAsyncOperation<StorageFolder> FolderHandle::GetStorageFolderAsync() const
	{
		co_return m_StorageItem.as<StorageFolder>();
	}
}