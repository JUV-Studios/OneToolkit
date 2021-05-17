#pragma once
#include "Storage.FileHandle.g.h"

using SHGDNF = DWORD;

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct FileHandle : FileHandleT<FileHandle>
        {
        public:
            FileHandle(Windows::Storage::StorageFile const& file, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            FileHandle(Windows::Storage::StorageFolder const& folder, hstring const& fileName, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            hstring Name() const noexcept;
            hstring DisplayName() const noexcept;
            FileSharingMode SharingMode() const noexcept;
            Windows::Storage::FileAccessMode AccessMode() const noexcept;
            Windows::Storage::StorageFile UnderlyingFile() const noexcept;
            Windows::Storage::StorageFolder Parent() const noexcept;
            uint64_t Value() const noexcept;
            uint64_t FileSize();
            Windows::Foundation::IAsyncAction InitializeAsync() noexcept;
            FileProperties GetFileProperties();
        private:
            hstring m_FileName;
            file_handle m_Handle;
            FileSharingMode m_SharingMode;
            Windows::Storage::FileAccessMode m_AccessMode;
            Windows::Storage::StorageFile m_UnderlyingFile = nullptr;
            Windows::Storage::StorageFolder m_ParentFolder = nullptr;
        };
    }

    namespace factory_implementation
    {
        struct FileHandle : FileHandleT<FileHandle, implementation::FileHandle>
        {
        };
    }
}