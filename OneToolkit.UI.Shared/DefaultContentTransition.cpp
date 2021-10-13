#include "pch.h"
#include "DefaultContentTransition.h"

using namespace Framework;
using namespace Framework::Controls;
using namespace Component::Media::Animation;

void DefaultContentTransition::SetContent(ContentPresenter^ presenter, UIElement^ content, NavigationType navigationType)
{
	presenter->Content = content;
}