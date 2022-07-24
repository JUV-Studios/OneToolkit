#include "pch.h"
#include <cstddef>
#include <sstream>
#include <iomanip>
#include "FileVersionInfo.h"

using namespace Platform;
using namespace VersionRT;

bool GetTranslationId(void* lpData, uint32 unBlockSize, uint16 wLangId, uint32& dwId, bool bPrimaryEnough)
{
	uint16* lpwData = nullptr;
	for (lpwData = (uint16*)lpData; (uint8*)lpwData < ((uint8*)lpData) + unBlockSize; lpwData += 2)
	{
		if (*lpwData == wLangId)
		{
			dwId = reinterpret_cast<uint32&>(*lpwData);
			return true;
		}
	}

	if (!bPrimaryEnough)
		return false;

	for (lpwData = (uint16*)lpData; (uint8*)lpwData < ((uint8*)lpData) + unBlockSize; lpwData += 2)
	{
		if (((*lpwData) & 0x00FF) == (wLangId & 0x00FF))
		{
			dwId = reinterpret_cast<uint32&>(*lpwData);
			return true;
		}
	}

	return false;
}

std::pair<void*, uint32> QueryValue(std::byte const* block, std::wstring_view subBlock)
{
	std::pair<void*, uint32> info;
	if (!VerQueryValue(block, subBlock.data(), &info.first, &info.second)) //throw ref new FailureException();
	/* else*/ return info;
}

std::wstring ToKey(StringInfoField value)
{
	switch (value)
	{
	case StringInfoField::Comments: return L"Comments";
	case StringInfoField::CompanyName: return L"CompanyName";
	case StringInfoField::FileDescription: return L"FileDescription";
	case StringInfoField::FileVersion: return L"FileVersion";
	case StringInfoField::InternalName: return L"InternalName";
	case StringInfoField::LegalCopyright: return L"LegalCopyright";
	case StringInfoField::LegalTrademarks: return L"LegalTrademarks";
	case StringInfoField::OriginalFilename: return L"OriginalFilename";
	case StringInfoField::PrivateBuild: return L"PrivateBuild";
	case StringInfoField::ProductName: return L"ProductName";
	case StringInfoField::ProductVersion: return L"ProductVersion";
	case StringInfoField::SpecialBuild: return L"SpecialBuild";
	default: return L"";
	}
}

uint32 ResolveLanguageCode(std::pair<void*, uint32> info)
{
	uint32 languageCode = 0;
	if (!GetTranslationId(info.first, info.second, GetUserDefaultLangID(), languageCode, false))
	{
		if (!GetTranslationId(info.first, info.second, GetUserDefaultLangID(), languageCode, true))
		{
			if (!GetTranslationId(info.first, info.second, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), languageCode, true))
			{
				if (!GetTranslationId(info.first, info.second, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), languageCode, true))
					languageCode = *reinterpret_cast<uint32*>(info.first);
			}
		}
	}

	return languageCode;
}

auto ComposeLangugeCodeToken(uint32 languageCode)
{
	std::wstringstream ss;
	ss << std::fixed << std::uppercase << std::setfill(L'0') << std::setw(4) << std::hex
		<< (languageCode & 0x0000FFFF) << std::setfill(L'0') << std::setw(4) << ((languageCode & 0xFFFF0000) >> 16) << L"\\";

	return ss.str();
}

FileVersionInfo^ FileVersionInfo::GetVersionInfo(String^ fileName)
{
	auto const result = ref new FileVersionInfo();
	DWORD handle;
	if (auto const versionInfoSize = GetFileVersionInfoSize(fileName->Data(), &handle))
	{
		auto const versionInfoData = std::make_unique<std::byte[]>(versionInfoSize);
		if (GetFileVersionInfo(fileName->Data(), handle, versionInfoSize, versionInfoData.get()) != FALSE)
		{
			auto const [fixedInfoData, fixedInfoSize] = QueryValue(versionInfoData.get(), L"\\");
			if (fixedInfoSize == sizeof(VS_FIXEDFILEINFO))
			{
				result->m_FixedInfo = *reinterpret_cast<VS_FIXEDFILEINFO*>(fixedInfoData);
			}

			auto const translationInfo = QueryValue(versionInfoData.get(), L"\\VarFileInfo\\Translation");
			auto const blockPrefix = L"\\StringFileInfo\\" + ComposeLangugeCodeToken(ResolveLanguageCode(translationInfo));
			for (uint8 index = 0; index < 12; ++index)
			{
				auto const stringInfo = QueryValue(versionInfoData.get(), (blockPrefix + ToKey(StringInfoField(index))));
				result->m_StringInfo[index] = ref new String(reinterpret_cast<wchar_t const*>(stringInfo.first), stringInfo.second);
			}
		}
	}

	return result;
}