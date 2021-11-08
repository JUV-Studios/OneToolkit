#include "pch.h"
#include "MachineInformation.h"
#include "System.MachineInformation.g.cpp"
#include <Inspectable.h>

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Security::ExchangeActiveSyncProvisioning;

namespace winrt::OneToolkit::System::implementation
{
	__interface __declspec(uuid("76E915B1-FF36-407C-9F57-160D3E540747")) IAnalyticsVersionInfo2 : ::IInspectable
	{
		int __stdcall ProductName(void** value) noexcept;
	};

	EasClientDeviceInformation clientDeviceInformation = nullptr;

	hstring MachineInformation::DeviceName()
	{
		return ClientDeviceInformation().FriendlyName();
	}

	hstring MachineInformation::DeviceModel()
	{
		return ClientDeviceInformation().SystemProductName();
	}

	hstring MachineInformation::DeviceFamily()
	{
		return AnalyticsInfo::VersionInfo().DeviceFamily();
	}

	hstring MachineInformation::DeviceManufacturer()
	{
		return ClientDeviceInformation().SystemManufacturer();
	}

	ProcessorArchitectureInfo MachineInformation::ArchitectureInfo()
	{
		ProcessorArchitectureInfo result;
		uint16 nativeMachine;
		uint16 processMachine;
		check_bool(IsWow64Process2(GetCurrentProcess(), &processMachine, &nativeMachine));
		switch (nativeMachine)
		{
		case IMAGE_FILE_MACHINE_I386:
			result.SystemArchitecture = ProcessorArchitecture::X86;
			break;
		case IMAGE_FILE_MACHINE_AMD64:
			result.SystemArchitecture = ProcessorArchitecture::X64;
			break;
		case IMAGE_FILE_MACHINE_ARM: 
			result.SystemArchitecture = ProcessorArchitecture::Arm; 
			break;
		case IMAGE_FILE_MACHINE_ARMNT: 
			result.SystemArchitecture = ProcessorArchitecture::Arm;
			break;
		case IMAGE_FILE_MACHINE_ARM64:
			result.SystemArchitecture = ProcessorArchitecture::Arm64; 
			break;
		default:
			result.SystemArchitecture = ProcessorArchitecture::Unknown;
			break;
		}

		switch (processMachine)
		{
		case IMAGE_FILE_MACHINE_I386: 
			result.SystemArchitecture == ProcessorArchitecture::Arm64 ? ProcessorArchitecture::X86OnArm64 : ProcessorArchitecture::X86;
			break;
		case IMAGE_FILE_MACHINE_AMD64: 
			result.SystemArchitecture = ProcessorArchitecture::X64;
			break;
		case IMAGE_FILE_MACHINE_ARM:
			result.SystemArchitecture = ProcessorArchitecture::Arm;
			break;
		case IMAGE_FILE_MACHINE_ARMNT: 
			result.SystemArchitecture = ProcessorArchitecture::Arm;
			break;
		case IMAGE_FILE_MACHINE_ARM64:
			result.SystemArchitecture = ProcessorArchitecture::Arm64;
			break;
		default: 
			result.SystemArchitecture = ProcessorArchitecture::Neutral;
			break;
		}

		return result;
	}

	hstring MachineInformation::OperatingSystemName()
	{
		if (auto versionInfo2 = AnalyticsInfo::VersionInfo().try_as<IAnalyticsVersionInfo2>())
		{
			hstring result;
			check_hresult(versionInfo2->ProductName(put_abi(result)));
			return result;
		}
		else
		{
			return ClientDeviceInformation().OperatingSystem();
		}
	}

	PackageVersion MachineInformation::OperatingSystemVersion()
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

	EasClientDeviceInformation MachineInformation::ClientDeviceInformation()
	{
		if (!clientDeviceInformation) clientDeviceInformation = {};
		return clientDeviceInformation;
	}
}