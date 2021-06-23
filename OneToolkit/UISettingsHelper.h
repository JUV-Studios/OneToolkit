#pragma once
#include "UI.UISettingsHelper.g.h"

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		struct UISettingsHelper : UISettingsHelperT<UISettingsHelper>
		{
			UISettingsHelper() = delete;
			static Windows::UI::ViewManagement::UISettings Instance();
		};
	}

	namespace factory_implementation
	{
		struct UISettingsHelper : UISettingsHelperT<UISettingsHelper, implementation::UISettingsHelper>
		{
		};
	}
}