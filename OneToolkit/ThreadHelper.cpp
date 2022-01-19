#include "System.ThreadHelper.g.h"
#include <winrt/Windows.Foundation.Collections.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		using CreateToolhelp32Snapshot = void* (__stdcall*)(uint32, uint32);

		struct ThreadHelper : ThreadHelperT<ThreadHelper>
		{
			static IAsyncOperation<IVectorView<ThreadId>> EnumerateAsync()
			{
				DynamicLibrary kernel32{ L"Kernel32.dll" };

				return nullptr;
			}
		};
	}

	namespace factory_implementation
	{
		struct ThreadHelper : ThreadHelperT<ThreadHelper, implementation::ThreadHelper>
		{
		};
	}
}

#include "System.ThreadHelper.g.cpp"