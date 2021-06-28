#include "pch.h"
#include "SuppressContentTransition.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace OneToolkit::UI::Xaml::Media::Animation;

void SuppressContentTransition::SetContent(ContentPresenter^ presenter, UIElement^ content, NavigationType)
{
	presenter->ContentTransitions = nullptr;
	presenter->Content = content;
}
