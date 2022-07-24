#pragma once
#include "Controls.PivotViewItem.g.h"

namespace winrt::OneToolkit::UI::Xaml::Controls
{
	namespace implementation
	{
		struct PivotViewItem : PivotViewItemT<PivotViewItem>
		{
		};
	}

	namespace factory_implementation
	{
		struct PivotViewItem : PivotViewItemT<PivotViewItem, implementation::PivotViewItem>
		{
		};
	}
}