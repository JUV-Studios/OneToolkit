#pragma once
#include "ApplicationModel.PackageVersionHelper.g.h"

namespace winrt::OneToolkit::ApplicationModel
{
    namespace implementation
    {
        struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper>
        {
            PackageVersionHelper() = delete;
            static hstring ToFormattedString(Windows::ApplicationModel::PackageVersion packageVersion);
            static Windows::ApplicationModel::PackageVersion FromFormattedString(hstring const& formattedString);
        };
    }

    namespace factory_implementation
    {
        struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper, implementation::PackageVersionHelper>
        {
        };
    }
}