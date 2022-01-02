#pragma once
#include "Storage.FileHandle.g.h"
#include "Storage.FolderHandle.g.h"

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct StorageItemHandle
        {
        public:
            StorageItemHandle(Windows::Storage::IStorageItem const& storageItem, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            juv::auto_property<FileSharingMode> const SharingMode;
            juv::auto_property<Windows::Storage::FileAccessMode> const AccessMode;
            StorageItemId Id() const noexcept;
            void Delete();
        protected:
            file_handle m_ItemHandle;
            Windows::Storage::IStorageItem m_StorageItem;
        };

        struct FileHandle : FileHandleT<FileHandle>, StorageItemHandle
        {
            FileHandle(Windows::Storage::StorageFile const& file, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            FileHandle(Windows::Storage::StorageFolder const& folder, hstring const& fileName, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            juv::auto_property<hstring> const ItemName;
            hstring DisplayName() const;
            juv::uint64 FileSize() const;
            Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFile> GetStorageFileAsync() const;
        };

        struct FolderHandle : FolderHandleT<FolderHandle>, StorageItemHandle
        {
            FolderHandle(Windows::Storage::StorageFolder const& folder, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            hstring ItemName() const;
            hstring DisplayName() const;
            Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFolder> GetStorageFolderAsync() const;
        };
    }

    namespace factory_implementation
    {
        struct FileHandle : FileHandleT<FileHandle, implementation::FileHandle>
        {
        };

        struct FolderHandle : FolderHandleT<FolderHandle, implementation::FolderHandle>
        {
        };
    }
}