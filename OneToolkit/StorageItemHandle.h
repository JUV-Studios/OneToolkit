#pragma once
#include "Storage.FileHandle.g.h"
#include "Storage.FolderHandle.g.h"

struct _CMINVOKECOMMANDINFO
{
    DWORD  cbSize;
    DWORD  fMask;
    HWND   hwnd;
    LPCSTR lpVerb;
    LPCSTR lpParameters;
    LPCSTR lpDirectory;
    int    nShow;
    DWORD  dwHotKey;
    HANDLE hIcon;
};

struct OOPCMINVOKECOMMANDINFO
{
    DWORD cbSize;
    DWORD fMask;
    HWND hwnd;
    LPCSTR lpVerbString;
    LPCSTR lpParameters;
    LPCSTR lpDirectory;
    int nShow;
    DWORD dwHotKey;
    LPCSTR lpTitle;
    LPCWSTR lpVerbWString;
    LPCWSTR lpParametersW;
    LPCWSTR lpDirectoryW;
    LPCWSTR lpTitleW;
    POINT ptInvoke;
    UINT lpVerbInt;
    UINT lpVerbWInt;
};

using SHGDNF = DWORD;

using CMINVOKECOMMANDINFO = _CMINVOKECOMMANDINFO;

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct FileHandle : FileHandleT<FileHandle>
        {
        public:
            FileHandle(Windows::Storage::StorageFile const& file, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            FileHandle(Windows::Storage::StorageFolder const& folder, hstring const& fileName, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            juv::auto_property<hstring> const ItemName;
            juv::auto_property<FileSharingMode> const SharingMode;
            juv::auto_property<Windows::Storage::FileAccessMode> const AccessMode;
            hstring DisplayName() const;
            juv::uint64 FileSize() const;
            StorageItemId Id() const noexcept;
            void Delete();
            Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFile> GetStorageFileAsync() const;
        private:
            file_handle m_FileHandle;
            Windows::Storage::IStorageItem m_StorageItem;
        };

        struct FolderHandle : FolderHandleT<FolderHandle>
        {
        public:
            FolderHandle(Windows::Storage::StorageFolder const& folder, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            juv::auto_property<FileSharingMode> const SharingMode;
            juv::auto_property<Windows::Storage::FileAccessMode> const AccessMode;
            hstring ItemName() const;
            hstring DisplayName() const;
            StorageItemId Id() const noexcept;
            void Delete();
            Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFolder> GetStorageFolderAsync() const;
        private:
            file_handle m_FolderHandle;
            Windows::Storage::StorageFolder m_StorageFolder;
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