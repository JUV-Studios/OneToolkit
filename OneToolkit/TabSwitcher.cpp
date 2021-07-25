#include "pch.h"
#include "TabSwitcher.h"
#include "UI.Input.TabSwitcher.g.cpp"

using namespace juv;

namespace winrt::OneToolkit::UI::Input::implementation
{
	int TabSwitcher::GetNewSelectionIndex(uint32 currentIndex, uint32 collectionSize, bool reverse)
	{
		if (collectionSize <= 1) return currentIndex;
		else if (reverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
		else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
	}
}