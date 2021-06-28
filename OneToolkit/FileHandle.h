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
            DeclareAutoProperty(hstring, FileName, {});
            DeclareAutoProperty(FileSharingMode, SharingMode, {});
            DeclareAutoProperty(Windows::Storage::FileAccessMode, AccessMode, {});
            juv::uint64 FileSize() const;
            juv::uint64 UnderlyingHandle() const noexcept;
        private:
            file_handle m_FileHandle;
        };
    }

    namespace factory_implementation
    {
        struct FileHandle : FileHandleT<FileHandle, implementation::FileHandle>
        {
        };
    }
}