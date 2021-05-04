#pragma once
#include "AppModel.PackageHelper.g.h"

namespace winrt::OneToolkit::AppModel
{
    namespace implementation
    {
        struct PackageHelper : PackageHelperT<PackageHelper>
        {
            PackageHelper() = delete;
            static bool IsPackaged() noexcept;
        };
    }

    namespace factory_implementation
    {
        struct PackageHelper : PackageHelperT<PackageHelper, implementation::PackageHelper>
        {
        };
    }
}