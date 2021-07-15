#include "pch.h"
#include "MachineInformation.h"
#include "System.MachineInformation.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation::Metadata;

namespace winrt::OneToolkit::System::implementation
{
	hstring MachineInformation::DeviceFamily()
	{
		return AnalyticsInfo::VersionInfo().DeviceFamily();
	}

	hstring MachineInformation::OSName()
	{
		if (ApiInformation::IsPropertyPresent(name_of<AnalyticsVersionInfo>(), L"ProductName")) return AnalyticsInfo::VersionInfo().ProductName();
		else return L"Windows";
	}

	PackageVersion MachineInformation::OSVersion()
	{
		uint64 version = std::stoull(AnalyticsInfo::VersionInfo().DeviceFamilyVersion().data());
		return PackageVersion
		{
			.Major = static_cast<uint16>((version & 0xFFFF000000000000L) >> 48),
			.Minor = static_cast<uint16>((version & 0x0000FFFF00000000L) >> 32),
			.Build = static_cast<uint16>((version & 0x00000000FFFF0000L) >> 16),
			.Revision = static_cast<uint16>(version & 0x000000000000FFFFL)
		};
	}
}