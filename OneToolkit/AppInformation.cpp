#include "pch.h"
#include "AppInformation.h"
#include "ApplicationModel.AppInformation.g.cpp"

using namespace winrt;
using namespace Windows::ApplicationModel::Core;

namespace winrt::OneToolkit::ApplicationModel::implementation
{
	bool AppInformation::IsCoreApplication()
	{
		return CoreApplication::Views().Size() != 0;
	}
}