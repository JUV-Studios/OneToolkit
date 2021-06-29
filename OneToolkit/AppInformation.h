#pragma once
#include "ApplicationModel.AppInformation.g.h"

namespace winrt::OneToolkit::ApplicationModel
{
	namespace implementation
	{
		struct AppInformation : AppInformationT<AppInformation>
		{
			AppInformation() = delete;
			static bool IsCoreApplication();
		};
	}

	namespace factory_implementation
	{
		struct AppInformation : AppInformationT<AppInformation, implementation::AppInformation>
		{
		};
	}
}