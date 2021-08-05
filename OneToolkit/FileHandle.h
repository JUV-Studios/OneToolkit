#pragma once
#include "Storage.FileHandle.g.h"

using SHGDNF = DWORD;

typedef struct _CMINVOKECOMMANDINFO
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
} CMINVOKECOMMANDINFO;

typedef struct OOPCMINVOKECOMMANDINFO
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
} 	OOPCMINVOKECOMMANDINFO;

namespace winrt::OneToolkit::Storage
{
    namespace implementation
    {
        struct FileHandle : FileHandleT<FileHandle>
        {
        public:
            FileHandle(Windows::Storage::StorageFile const& file, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            FileHandle(Windows::Storage::StorageFolder const& folder, hstring const& fileName, Windows::Storage::FileAccessMode accessMode, FileSharingMode sharingMode);
            DeclareAutoProperty(hstring, ItemName, {});
            DeclareAutoProperty(FileSharingMode, SharingMode, {});
            DeclareAutoProperty(Windows::Storage::FileAccessMode, AccessMode, {});
            juv::uint64 FileSize() const;
            StorageItemId Id() const noexcept;
            void Delete();
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