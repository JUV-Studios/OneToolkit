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
            hstring FileName() const noexcept;
            juv::uint64 FileSize() const;
            juv::uint64 UnderlyingHandle() const noexcept;
            Windows::Storage::FileAccessMode AccessMode() const noexcept;
        private:
            hstring m_FileName;
            file_handle m_FileHandle;
            Windows::Storage::FileAccessMode m_AccessMode;
        };
    }

    namespace factory_implementation
    {
        struct FileHandle : FileHandleT<FileHandle, implementation::FileHandle>
        {
        };
    }
}