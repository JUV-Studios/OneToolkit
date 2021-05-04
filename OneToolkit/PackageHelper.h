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
            static Windows::Foundation::IAsyncOperation<bool> CheckIsFullTrustAsync();
            static Windows::Foundation::IAsyncOperation<Windows::Data::Xml::Dom::XmlDocument> GetPackageManifestAsync();
        };
    }

    namespace factory_implementation
    {
        struct PackageHelper : PackageHelperT<PackageHelper, implementation::PackageHelper>
        {
        };
    }
}