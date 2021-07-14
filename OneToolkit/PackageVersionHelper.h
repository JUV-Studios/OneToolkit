#pragma once
#include "ApplicationModel.PackageVersionHelper.g.h"

namespace winrt::OneToolkit::ApplicationModel
{
    namespace implementation
    {
        struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper>
        {
            PackageVersionHelper() = delete;
            static hstring GetFormattedString(Windows::ApplicationModel::PackageVersion packageVersion);
        };
    }

    namespace factory_implementation
    {
        struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper, implementation::PackageVersionHelper>
        {
        };
    }
}