#pragma once
#include <winrt/Windows.UI.Core.Preview.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.WindowManagement.Preview.h>
#include <winrt/Microsoft.UI.Windowing.h>

import juv;
import OneToolkit;

namespace winrt::OneToolkit::UI::Windowing::implementation
{
	namespace Universal
	{
		namespace Core
		{
			using namespace Windows::UI::Core;
			using namespace Windows::UI::Core::Preview;
			using namespace Windows::UI::ViewManagement;
			using namespace Windows::ApplicationModel::Core;
		}

		namespace Preview
		{
			using Windows::UI::Core::CoreVirtualKeyStates;
			using Windows::UI::Core::Preview::CoreAppWindowPreview;
			using namespace Windows::UI::WindowManagement;
			using namespace Windows::UI::WindowManagement::Preview;
		}
	}

	namespace Desktop
	{
		using namespace Microsoft::UI::Windowing;
		using Windows::UI::Core::CoreVirtualKeyStates;
	}
}