#pragma once
#include "UI.Input.TabSwitcher.g.h"

namespace winrt::OneToolkit::UI::Input
{
	namespace implementation
	{
		struct TabSwitcher : TabSwitcherT<TabSwitcher>
		{
			TabSwitcher() = delete;
			static int GetNewSelectionIndex(juv::uint32 currentIndex, juv::uint32 collectionSize, bool reverse);
		};
	}

	namespace factory_implementation
	{
		struct TabSwitcher : TabSwitcherT<TabSwitcher, implementation::TabSwitcher>
		{
		};
	}
}