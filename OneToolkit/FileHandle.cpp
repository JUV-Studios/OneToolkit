#include "pch.h"
#include "FileHandle.h"
#include <WindowsStorageCOM.h>
#include "Storage.FileHandle.g.cpp"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::Storage::implementation
{
    FileHandle::FileHandle(StorageFile const& file, FileAccessMode accessMode, FileSharingMode sharingMode) : m_UnderlyingFile(file), m_FileName(file.Name()), m_AccessMode(accessMode), m_SharingMode(sharingMode)
    {
        check_hresult(file.as<IStorageItemHandleAccess>()->Create(accessMode == FileAccessMode::Read ? HAO_READ | HAO_READ_ATTRIBUTES : HAO_READ | HAO_READ_ATTRIBUTES | HAO_WRITE | HAO_DELETE, static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), 
            HO_NONE, nullptr, m_Handle.put()));
    }

    FileHandle::FileHandle(StorageFolder const& folder, hstring const& fileName, FileAccessMode accessMode, FileSharingMode sharingMode) : m_ParentFolder(folder), m_FileName(fileName), m_AccessMode(accessMode), m_SharingMode(sharingMode)
    {
        check_hresult(folder.as<IStorageFolderHandleAccess>()->Create(fileName.data(), HCO_CREATE_NEW, accessMode == FileAccessMode::Read ? HAO_READ | HAO_READ_ATTRIBUTES : HAO_READ | HAO_READ_ATTRIBUTES | HAO_WRITE | HAO_DELETE, 
            static_cast<HANDLE_SHARING_OPTIONS>(sharingMode), HO_NONE, nullptr, m_Handle.put()));
    }

    hstring FileHandle::Name() const noexcept
    {
        return m_FileName;
    }

    hstring FileHandle::DisplayName() const noexcept
    {
        return m_UnderlyingFile ? m_UnderlyingFile.DisplayName() : m_FileName;
    }

    FileSharingMode FileHandle::SharingMode() const noexcept
    {
        return m_SharingMode;
    }

    FileAccessMode FileHandle::AccessMode() const noexcept
    {
        return m_AccessMode;
    }

    StorageFile FileHandle::UnderlyingFile() const noexcept
    {
        return m_UnderlyingFile;
    }

    StorageFolder FileHandle::Parent() const noexcept
    {
        return m_ParentFolder;
    }

    uint64_t FileHandle::FileSize()
    {
        LARGE_INTEGER result;
        check_bool(GetFileSizeEx(m_Handle.get(), &result));
        return result.QuadPart;
    }
     
    uint64_t FileHandle::Value() const noexcept
    {
        return static_cast<uint64_t>(reinterpret_cast<size_t>(m_Handle.get()));
    }

    IAsyncAction FileHandle::InitializeAsync() noexcept
    {
        if (!m_UnderlyingFile && m_ParentFolder)
        {
            try
            {
                m_UnderlyingFile = co_await m_ParentFolder.GetFileAsync(m_FileName);
            }
            catch (hresult_error const&)
            {
                m_UnderlyingFile = nullptr;
            }
        }
        else if (!m_ParentFolder && m_UnderlyingFile)
        {
            try
            {
                m_ParentFolder = co_await m_UnderlyingFile.GetParentAsync();
            }
            catch (hresult_error const&)
            {
                m_ParentFolder = nullptr;
            }
        }
    }

    FileProperties FileHandle::GetFileProperties()
    {
        FILETIME creationTime;
        FILETIME lastAccessTime;
        FILETIME lastWriteTime;
        check_bool(GetFileTime(m_Handle.get(), &creationTime, &lastAccessTime, &lastWriteTime));
        return FileProperties
        {
            .CreationTime = clock::from_file_time(creationTime),
            .LastAccessTime = clock::from_file_time(lastAccessTime),
            .LastWriteTime = clock::from_file_time(lastWriteTime)
        };
    }
}