#include "pch.h"
#include "IconSourceHelper.h"

using namespace Windows::UI::Xaml::Controls;
using namespace OneToolkit::UI::Controls;

IconSourceElement^ IconSourceHelper::CreateElement(IconSource^ iconSource)
{
	auto srcElement = ref new IconSourceElement;
	srcElement->IconSource = iconSource;
	return srcElement;
}