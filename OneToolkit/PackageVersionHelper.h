#pragma once
#include "System.PackageVersionHelper.g.h"

namespace winrt::OneToolkit::System
{
    namespace implementation
    {
        struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper>
        {
            PackageVersionHelper() = delete;
            static hstring Stringify(Windows::ApplicationModel::PackageVersion packageVersion);
            static Windows::ApplicationModel::PackageVersion Parse(hstring const& formattedString);
        };
    }

    namespace factory_implementation
    {
        struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper, implementation::PackageVersionHelper>
        {
        };
    }
}