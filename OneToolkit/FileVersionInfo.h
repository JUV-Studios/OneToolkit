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

#pragma once

#include <windows.h>
#include <string>
#include <chrono>
#include <map>

namespace FileVersion
{
#ifdef UNICODE
    using char_type = wchar_t;
    #define FILE_INFO_TEXT(T) L##T
#else
    using char_type = char;
    #define FILE_INFO_TEXT(T) T
#endif

    using string_type = std::basic_string<char_type>;
    using stringstream_type = std::basic_stringstream<char_type>;

    enum class StringInfoField
    {
          Undefined
        , FileDescription
        , FileVersion
        , InternalName
        , LegalCopyright
        , OriginalFileName
        , ProductName
        , ProductVersion
        , Comments
        , LegalTrademarks
        , PrivateBuild
        , SpecialBuild
    };

#pragma pack(push,1)
    struct VersionQuadWord
    {
        uint16_t minor;
        uint16_t major;
        uint16_t revision;
        uint16_t build;
    };
#pragma pack(pop)

    class FileVersionInfo final
    {
    public: // methods
        bool Init(const string_type& fileName);
        const VersionQuadWord& GetFileVersion() const;
        const VersionQuadWord& GetProductVersion() const;
        const VS_FIXEDFILEINFO& GetFileInfo() const;
        std::chrono::system_clock::time_point GetFileDate() const;
        const string_type& GetInfoString(StringInfoField type) const;

    private: // methods
        BOOL GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough = FALSE);
        std::chrono::system_clock::time_point FileTime2TimePoint(const FILETIME& ft) const;
        DWORD ResolveLanguageCode(LPVOID lpInfo, UINT unInfoLen);
        string_type ComposeLangugeCodeToken(DWORD languageCode) const;
        
    private: //member fields
        std::map<StringInfoField, string_type> mMapNameToValue;
        VS_FIXEDFILEINFO mFileInfo{};
    };
}