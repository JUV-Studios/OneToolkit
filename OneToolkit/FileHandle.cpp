#include "pch.h"
#include "FileHandle.h"
#include "Storage.FileHandle.g.cpp"
#include <WindowsStorageCOM.h>

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;

namespace winrt::OneToolkit::Storage::implementation
{
	inline auto AsAccessOption(FileAccessMode accessMode)
	{
		return accessMode == FileAccessMode::Read ? HAO_READ | HAO_READ_ATTRIBUTES : HAO_READ | HAO_READ_ATTRIBUTES | HAO_WRITE | HAO_DELETE;
	}

	FileHandle::FileHandle(StorageFile const& file, FileAccessMode accessMode, FileSharingMode sharingMode) : m_FileName(file.Name()), m_AccessMode(accessMode)
	{
		check_hresult(file.as<IStorageItemHandleAccess>()->Create(AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr, m_FileHandle.put()));
	}

	FileHandle::FileHandle(StorageFolder const& folder, hstring const& fileName, FileAccessMode accessMode, FileSharingMode sharingMode) : m_FileName(fileName), m_AccessMode(accessMode)
	{
		check_hresult(folder.as<IStorageFolderHandleAccess>()->Create(fileName.data(), HCO_CREATE_NEW, AsAccessOption(accessMode), static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr,
			m_FileHandle.put()));
	}

	hstring FileHandle::FileName() const noexcept
	{
		return m_FileName;
	}

	uint64 FileHandle::FileSize() const
	{
		LARGE_INTEGER result;
		check_bool(GetFileSizeEx(m_FileHandle.get(), &result));
		return result.QuadPart;
	}

	uint64 FileHandle::UnderlyingHandle() const noexcept
	{
		return juv::as_value<int64>(m_FileHandle.get());
	}

	FileAccessMode FileHandle::AccessMode() const noexcept
	{
		return m_AccessMode;
	}
}