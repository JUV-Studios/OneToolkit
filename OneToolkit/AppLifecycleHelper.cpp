#include "System.AppLifecycleHelper.g.h"
#include <winrt/Windows.System.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt;
using namespace Windows::System;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct AppLifecycleHelper : AppLifecycleHelperT<AppLifecycleHelper>
		{
			AppLifecycleHelper() = delete;

			static IAsyncAction SuspendAsync()
			{
				check_hresult((co_await (co_await AppDiagnosticInfo::RequestInfoForAppAsync()).GetAt(0).GetResourceGroups().GetAt(0).StartSuspendAsync()).ExtendedError());
			}
		};
	}

	namespace factory_implementation
	{
		struct AppLifecycleHelper : AppLifecycleHelperT<AppLifecycleHelper, implementation::AppLifecycleHelper>
		{
		};
	}
}

#include "System.AppLifecycleHelper.g.cpp"