// Code adapted from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/main/Microsoft.Toolkit.Uwp/Helpers/SystemInformation.cs.

#include "System.SystemInformation.g.h"
#include "System.OperatingSystemInfo.g.h"
#include <Windows.h>
#include <wil/resource.h>
#include <winrt/Windows.System.Profile.h>
#include <winrt/Windows.Security.ExchangeActiveSyncProvisioning.h>

import juv;
import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Security::ExchangeActiveSyncProvisioning;

wchar_t const* BrandingFormatString(wchar_t const* format) noexcept
{
	wil::unique_hmodule const winBrand{ LoadLibrary(L"WinBrand.dll") };
	auto const brandingFormatString = reinterpret_cast<wchar_t const* (__stdcall*)(wchar_t const*)>(GetProcAddress(winBrand.get(), "BrandingFormatString"));
	return brandingFormatString(format);
}

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct OperatingSystemInfo : OperatingSystemInfoT<OperatingSystemInfo>, WeakSingleton<OperatingSystemInfo>
		{
		public:
			auto_property<hstring> const Name{ GetName() };

			auto_property<PackageVersion> const Version{ GetVersion() };
		private:
			static hstring GetName()
			{
				return BrandingFormatString(L"%WINDOWS_LONG%");
			}

			static PackageVersion GetVersion()
			{
				auto const versionInfo = std::stoull(AnalyticsInfo::VersionInfo().DeviceFamilyVersion().data());
				return PackageVersion
				{
					.Major = static_cast<uint16>((versionInfo & 0xFFFF000000000000L) >> 48),
					.Minor = static_cast<uint16>((versionInfo & 0x0000FFFF00000000L) >> 32),
					.Build = static_cast<uint16>((versionInfo & 0x00000000FFFF0000L) >> 16),
					.Revision = static_cast<uint16>(versionInfo & 0x000000000000FFFFL)
				};
			}
		};

		struct SystemInformation : SystemInformationT<SystemInformation>
		{
			inline static auto_property<EasClientDeviceInformation> const ClientDeviceInformation;

			static hstring DeviceName()
			{
				return ClientDeviceInformation().FriendlyName();
			}

			static hstring DeviceModel()
			{
				return ClientDeviceInformation().SystemProductName();
			}

			static hstring DeviceFamily()
			{
				return AnalyticsInfo::VersionInfo().DeviceFamily();
			}

			static hstring DeviceManufacturer()
			{
				return ClientDeviceInformation().SystemManufacturer();
			}

			static auto OperatingSystem()
			{
				return OperatingSystemInfo::Instance();
			}
		};
	}

	namespace factory_implementation
	{
		struct SystemInformation : SystemInformationT<SystemInformation, implementation::SystemInformation>
		{
		};
	}
}

#include "System.SystemInformation.g.cpp"