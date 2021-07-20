#pragma once
#include "System.ApplianceInformation.g.h"

namespace winrt::OneToolkit::System
{
    namespace implementation
    {
        struct ApplianceInformation : ApplianceInformationT<ApplianceInformation>
        {
            ApplianceInformation() = delete;
            static hstring DeviceName();
            static hstring DeviceModel();
            static hstring DeviceFamily();
            static hstring DeviceManufacturer();
            static hstring OperatingSystem();
            static Windows::ApplicationModel::PackageVersion SoftwareVersion();
            static Windows::System::ProcessorArchitecture HardwareArchitecture();
        };
    }
    
    namespace factory_implementation
    {
        struct ApplianceInformation : ApplianceInformationT<ApplianceInformation, implementation::ApplianceInformation>
        {
        };
    }
}