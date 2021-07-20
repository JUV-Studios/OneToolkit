#include "pch.h"
#include "ApplianceInformation.h"
#include "System.ApplianceInformation.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Security::ExchangeActiveSyncProvisioning;
using namespace OneToolkit::ApplicationModel;

namespace winrt::OneToolkit::System::implementation
{
	EasClientDeviceInformation ClientDeviceInformation;

	hstring ApplianceInformation::DeviceName()
	{
		return ClientDeviceInformation.FriendlyName();
	}

	hstring ApplianceInformation::DeviceModel()
	{
		return ClientDeviceInformation.SystemProductName();
	}

	hstring ApplianceInformation::DeviceFamily()
	{
		return AnalyticsInfo::VersionInfo().DeviceFamily();
	}

	hstring ApplianceInformation::DeviceManufacturer()
	{
		return ClientDeviceInformation.SystemManufacturer();
	}

	hstring ApplianceInformation::OperatingSystem()
	{
		return ApiInformation::IsPropertyPresent(name_of<AnalyticsVersionInfo>(), L"ProductName") ? AnalyticsInfo::VersionInfo().ProductName() : ClientDeviceInformation.OperatingSystem();
	}

	PackageVersion ApplianceInformation::SoftwareVersion()
	{
		auto versionInfo = std::stoull(AnalyticsInfo::VersionInfo().DeviceFamilyVersion().data());
		return PackageVersion
		{
			.Major = static_cast<uint16>((versionInfo & 0xFFFF000000000000L) >> 48),
			.Minor = static_cast<uint16>((versionInfo & 0x0000FFFF00000000L) >> 32),
			.Build = static_cast<uint16>((versionInfo & 0x00000000FFFF0000L) >> 16),
			.Revision = static_cast<uint16>(versionInfo & 0x000000000000FFFFL)
		};
	}

	ProcessorArchitecture ApplianceInformation::HardwareArchitecture()
	{
		return Package::Current().Id().Architecture();
	}
}