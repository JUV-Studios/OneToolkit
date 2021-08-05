#include "pch.h"
#include "TabSwitcher.h"
#include "UI.Input.TabSwitcher.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::UI::Input::implementation
{
	uint32 TabSwitcher::GetNewSelectionIndex(uint32 currentIndex, uint32 collectionSize, bool reverse) noexcept
	{
		if (collectionSize <= 1) return currentIndex;
		else if (reverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
		else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
	}
}