#pragma once
#include "System.MachineInformation.g.h"

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct MachineInformation : MachineInformationT<MachineInformation>
		{
			MachineInformation() = delete;
			static hstring DeviceFamily();
			static hstring OSName();
			static Windows::ApplicationModel::PackageVersion OSVersion();
		};
	}

	namespace factory_implementation
	{
		struct MachineInformation : MachineInformationT<MachineInformation, implementation::MachineInformation>
		{
		};
	}
}