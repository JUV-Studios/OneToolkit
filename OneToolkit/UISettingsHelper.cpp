#include "pch.h"
#include "UISettingsHelper.h"
#include "UI.UISettingsHelper.g.cpp"

using namespace winrt;
using namespace Windows::UI::ViewManagement;

namespace winrt::OneToolkit::UI::implementation
{
	UISettings UISettingsHelper::Instance()
	{
		static slim_mutex instanceLock;
		const slim_lock_guard lockGuard { instanceLock };
		static UISettings instance = nullptr;
		if (instance == nullptr) instance = {};
		return instance;
	}
}