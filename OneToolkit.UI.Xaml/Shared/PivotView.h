#pragma once
#include "Controls.PivotView.g.h"

namespace winrt::OneToolkit::UI::Xaml::Controls
{
	namespace implementation
	{
		struct PivotView : PivotViewT<PivotView>
		{
			PivotView();
		};
	}

	namespace factory_implementation
	{
		struct PivotView : PivotViewT<PivotView, implementation::PivotView>
		{
		};
	}
}