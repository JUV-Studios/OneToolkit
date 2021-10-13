#pragma once
#include "System.MachineInformation.g.h"

namespace winrt::OneToolkit::System
{
    namespace implementation
    {
        struct MachineInformation : MachineInformationT<MachineInformation>
        {
            MachineInformation() = delete;
            static hstring DeviceName();
            static hstring DeviceModel();
            static hstring DeviceFamily();
            static hstring DeviceManufacturer();
            static hstring OperatingSystem();
            static Windows::ApplicationModel::PackageVersion SoftwareVersion();
            static Windows::System::ProcessorArchitecture ProcessorArchitecture();
            static Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation ClientDeviceInformation();
        };
    }
    
    namespace factory_implementation
    {
        struct MachineInformation : MachineInformationT<MachineInformation, implementation::MachineInformation>
        {
        };
    }
}