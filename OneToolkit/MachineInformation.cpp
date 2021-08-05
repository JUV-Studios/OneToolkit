#include "pch.h"
#include "MachineInformation.h"
#include "System.MachineInformation.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Security::ExchangeActiveSyncProvisioning;
using namespace OneToolkit::ApplicationModel;

namespace winrt::OneToolkit::System::implementation
{
	EasClientDeviceInformation ClientDeviceInformation;

	__interface __declspec(uuid("AF86E2E0-B12D-4c6a-9C5A-D7AA65101E90")) abi_IInspectable : ::IUnknown
	{
		int __stdcall GetIids(uint32_t* count, guid** ids) noexcept;
		int __stdcall GetRuntimeClassName(void** name) noexcept;
		int __stdcall GetTrustLevel(TrustLevel* level) noexcept;
	};

	__interface __declspec(uuid("76E915B1-FF36-407C-9F57-160D3E540747")) IAnalyticsVersionInfo2 : abi_IInspectable
	{
		int __stdcall ProductName(void** value) noexcept;
	};

	hstring MachineInformation::DeviceName()
	{
		return ClientDeviceInformation.FriendlyName();
	}

	hstring MachineInformation::DeviceModel()
	{
		return ClientDeviceInformation.SystemProductName();
	}

	hstring MachineInformation::DeviceFamily()
	{
		return AnalyticsInfo::VersionInfo().DeviceFamily();
	}

	hstring MachineInformation::DeviceManufacturer()
	{
		return ClientDeviceInformation.SystemManufacturer();
	}

	hstring MachineInformation::OperatingSystem()
	{
		if (auto versionInfo2 = AnalyticsInfo::VersionInfo().try_as<IAnalyticsVersionInfo2>())
		{
			hstring result;
			check_hresult(versionInfo2->ProductName(put_abi(result)));
			return result;
		}
		else
		{
			return ClientDeviceInformation.OperatingSystem();
		}
	}

	PackageVersion MachineInformation::SoftwareVersion()
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

	ProcessorArchitecture MachineInformation::HardwareArchitecture()
	{
		return Package::Current().Id().Architecture();
	}
}