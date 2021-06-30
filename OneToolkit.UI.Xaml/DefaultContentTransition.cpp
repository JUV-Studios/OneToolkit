#include "pch.h"
#include "DefaultContentTransition.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace OneToolkit::UI::Xaml::Media::Animation;

void DefaultContentTransition::SetContent(ContentPresenter^ presenter, UIElement^ content, NavigationType)
{
	presenter->Content = content;
}
