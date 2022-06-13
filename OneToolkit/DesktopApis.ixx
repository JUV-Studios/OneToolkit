module;

#include <wil/resource.h>

export module DesktopApis;

export int GetSystemMetrics(int nIndex) noexcept
{
	wil::unique_hmodule const module{ LoadLibrary(L"ext-ms-win-ntuser-sysparams-ext-l1-1-0.dll") };
	auto const getSystemMetrics = reinterpret_cast<int(__stdcall*)(int)>(GetProcAddress(module.get(), "GetSystemMetrics"));
	return getSystemMetrics(nIndex);
}

export wchar_t const* BrandingFormatString(wchar_t const* format) noexcept
{
	wil::unique_hmodule const winBrand{ LoadLibrary(L"WinBrand.dll") };
	auto const brandingFormatString = reinterpret_cast<wchar_t const* (__stdcall*)(wchar_t const*)>(GetProcAddress(winBrand.get(), "BrandingFormatString"));
	return brandingFormatString(format);
}