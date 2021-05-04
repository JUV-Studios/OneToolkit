#include "pch.h"
#include "PackageHelper.h"
#include "AppModel.PackageHelper.g.cpp"

namespace winrt::OneToolkit::AppModel::implementation
{
	bool PackageHelper::IsPackaged() noexcept
	{
		std::array<wchar_t, 64> packageFamilyName;
		uint32_t retrivedSize = 64;
		return GetPackageFamilyName(GetCurrentProcess(), &retrivedSize, packageFamilyName.data()) != APPMODEL_ERROR_NO_PACKAGE;
	}
}