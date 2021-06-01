#pragma once
#include "Storage.FileHandle.g.h"

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct FileHandle : FileHandleT<FileHandle>
        {
        public:
            FileHandle(Windows::Storage::StorageFile const& file, Windows::Storage::FileAccessMode accessMode);
            Windows::Storage::FileAccessMode AccessMode() const noexcept;
        private:
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