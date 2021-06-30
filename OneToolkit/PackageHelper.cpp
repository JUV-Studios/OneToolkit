#include "pch.h"
#include "PackageHelper.h"
#include "ApplicationModel.PackageHelper.g.cpp"

using namespace juv;

namespace winrt::OneToolkit::ApplicationModel::implementation
{
	bool PackageHelper::IsPackaged() noexcept
	{
		PACKAGE_ID packageId;
		uint32 bufferSize = sizeof(packageId);
		return GetCurrentPackageId(&bufferSize, reinterpret_cast<uint8*>(&packageId)) == ERROR_SUCCESS;
	}
}