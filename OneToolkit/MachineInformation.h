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
            static ProcessorArchitectureInfo ArchitectureInfo();
            static hstring OperatingSystemName();
            static Windows::ApplicationModel::PackageVersion OperatingSystemVersion();
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