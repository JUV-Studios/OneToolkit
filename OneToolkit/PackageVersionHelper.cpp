#include "pch.h"
#include "PackageVersionHelper.h"
#include "ApplicationModel.PackageVersionHelper.g.cpp"

using namespace winrt;
using namespace Windows::ApplicationModel;

namespace winrt::OneToolkit::ApplicationModel::implementation
{
	hstring PackageVersionHelper::GetFormattedString(PackageVersion packageVersion)
	{
		return hstring(std::format(L"{0}.{1}.{2}.{3}", packageVersion.Major, packageVersion.Minor, packageVersion.Build, packageVersion.Revision));
	}
}