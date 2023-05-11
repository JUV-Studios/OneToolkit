/*
-----------------------------------------------------------------------------
Copyright (c) 2018 Lior Lahav

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "FileVersionInfo.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>

namespace FileVersion
{

    const std::map<StringInfoField, string_type> sMapNameToKey =
    {
       { StringInfoField::Undefined       , FILE_INFO_TEXT("Undefined")}
     , { StringInfoField::FileDescription , FILE_INFO_TEXT("FileDescription") }
     , { StringInfoField::FileVersion     , FILE_INFO_TEXT("FileVersion") }
     , { StringInfoField::InternalName    , FILE_INFO_TEXT("InternalName") }
     , { StringInfoField::OriginalFileName, FILE_INFO_TEXT("OriginalFileName") }
     , { StringInfoField::ProductName     , FILE_INFO_TEXT("ProductName") }
     , { StringInfoField::ProductVersion  , FILE_INFO_TEXT("ProductVersion") }
     , { StringInfoField::Comments        , FILE_INFO_TEXT("Comments") }
     , { StringInfoField::LegalTrademarks , FILE_INFO_TEXT("LegalTrademarks") }
     , { StringInfoField::LegalCopyright  , FILE_INFO_TEXT("LegalCopyright") }
     , { StringInfoField::PrivateBuild    , FILE_INFO_TEXT("PrivateBuild") }
     , { StringInfoField::SpecialBuild    , FILE_INFO_TEXT("SpecialBuild") }

    };

    BOOL FileVersionInfo::GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough/*= FALSE*/)
    {
        LPWORD lpwData = nullptr;
        for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData) + unBlockSize; lpwData += 2)
        {
            if (*lpwData == wLangId)
            {
                dwId = *((DWORD*)lpwData);
                return TRUE;
            }
        }

        if (!bPrimaryEnough)
            return FALSE;

        for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData) + unBlockSize; lpwData += 2)
        {
            if (((*lpwData) & 0x00FF) == (wLangId & 0x00FF))
            {
                dwId = *((DWORD*)lpwData);
                return TRUE;
            }
        }

        return FALSE;
    }

    std::chrono::system_clock::time_point FileVersionInfo::FileTime2TimePoint(const FILETIME& ft) const
    {
        SYSTEMTIME st = { 0 };
        if (!FileTimeToSystemTime(&ft, &st))
            return std::chrono::system_clock::time_point((std::chrono::system_clock::time_point::min)());

        // number of seconds 
        ULARGE_INTEGER ull;
        ull.LowPart = ft.dwLowDateTime;
        ull.HighPart = ft.dwHighDateTime;

        time_t secs = ull.QuadPart / 10000000ULL - 11644473600ULL;
        std::chrono::milliseconds ms((ull.QuadPart / 10000ULL) % 1000);

        auto tp = std::chrono::system_clock::from_time_t(secs);
        tp += ms;
        return tp;
    }

    DWORD FileVersionInfo::ResolveLanguageCode(LPVOID lpInfo, UINT unInfoLen)
    {
        DWORD languageCode = 0;
        if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), languageCode, FALSE))
        {
            if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), languageCode, TRUE))
            {
                if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), languageCode, TRUE))
                {
                    if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), languageCode, TRUE))
                        languageCode = *reinterpret_cast<DWORD*>(lpInfo);
                }
            }
        }
        return languageCode;
    }

    string_type FileVersionInfo::ComposeLangugeCodeToken(DWORD languageCode) const
    {
        stringstream_type ss;
              ss << std::fixed << std::uppercase << std::setfill(FILE_INFO_TEXT('0')) << std::setw(4) << std::hex
                  << (languageCode & 0x0000FFFF) << std::setfill(FILE_INFO_TEXT('0')) << std::setw(4) << ((languageCode & 0xFFFF0000) >> 16) << FILE_INFO_TEXT("\\");

              return ss.str();

    }

    bool FileVersionInfo::Init(const string_type& fileName)
    {
        mFileInfo = {};
        mMapNameToValue.clear();

        bool success = false;


        DWORD dwHandle;
        const DWORD dwFileVersionInfoSize = GetFileVersionInfoSize(fileName.c_str(), &dwHandle);
        if (dwFileVersionInfoSize > 0)
        {
            std::unique_ptr<std::byte[]> lpData = std::make_unique<std::byte[]>(dwFileVersionInfoSize);

            if (GetFileVersionInfo(fileName.c_str(), dwHandle, dwFileVersionInfoSize, lpData.get()) != FALSE)
            {
                // Get root file info 
                LPVOID lpInfo;
                UINT unInfoLen;

                if (VerQueryValue(lpData.get(), FILE_INFO_TEXT("\\"), &lpInfo, &unInfoLen))
                {
                    if (unInfoLen == sizeof(mFileInfo))
                        memcpy(&mFileInfo, lpInfo, unInfoLen);
                }

                
                VerQueryValue(lpData.get(), FILE_INFO_TEXT("\\VarFileInfo\\Translation"), &lpInfo, &unInfoLen);

                const string_type blockPrefix = FILE_INFO_TEXT("\\StringFileInfo\\") + ComposeLangugeCodeToken(ResolveLanguageCode(lpInfo, unInfoLen));
                //Get all string info fields
                for (auto const&[key, value] : sMapNameToKey)
                {
                    if (VerQueryValue(lpData.get(), (blockPrefix + value).c_str(), &lpInfo, &unInfoLen))
                        mMapNameToValue.emplace(key, reinterpret_cast<char_type*>(lpInfo));
                }
                
                success = true;
            }
        }
        return success;
    }

    const VersionQuadWord& FileVersionInfo::GetFileVersion() const
    {
        return *reinterpret_cast<const VersionQuadWord*>(&mFileInfo.dwFileVersionMS );
    }

    const VersionQuadWord& FileVersionInfo::GetProductVersion() const
    {
        return *reinterpret_cast<const VersionQuadWord*>(&mFileInfo.dwProductVersionMS);
    }

    const VS_FIXEDFILEINFO& FileVersionInfo::GetFileInfo() const
    {
        return mFileInfo;
    }

    std::chrono::system_clock::time_point FileVersionInfo::GetFileDate() const
    {
        FILETIME	ft;
        ft.dwLowDateTime = mFileInfo.dwFileDateLS;
        ft.dwHighDateTime = mFileInfo.dwFileDateMS;
        return FileTime2TimePoint(ft);
    }

    const string_type& FileVersionInfo::GetInfoString(StringInfoField type) const
    {
        static string_type emptyString;
        auto it = mMapNameToValue.find(type);
        return it != mMapNameToValue.end() ? it->second : emptyString;
    }
}
