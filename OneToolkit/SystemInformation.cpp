#include "System.SystemInformation.g.h"
#include <Windows.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.System.Profile.h>
#include <winrt/Windows.Security.ExchangeActiveSyncProvisioning.h>

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Security::ExchangeActiveSyncProvisioning;

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct SystemInformation : static_t, SystemInformationT<SystemInformation>
		{
		public:
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

			static ProcessorArchitectureInfo ArchitectureInfo()
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

			static hstring OperatingSystemName()
			{
				return ApiInformation::IsPropertyPresent(name_of<AnalyticsVersionInfo>(), L"ProductName") ? AnalyticsInfo::VersionInfo().ProductName() : ClientDeviceInformation().OperatingSystem();
			}

			static PackageVersion OperatingSystemVersion()
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

			static 	EasClientDeviceInformation ClientDeviceInformation()
			{
				static slim_mutex mutex;
				slim_lock_guard const mutexLock{ mutex };
				if (!m_ClientDeviceInformation) m_ClientDeviceInformation = {};
				return m_ClientDeviceInformation;
			}
		private:
			inline static EasClientDeviceInformation m_ClientDeviceInformation = nullptr;
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