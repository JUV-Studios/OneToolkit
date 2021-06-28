#include "pch.h"
#include "SlideContentTransition.h"

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace OneToolkit::UI::Xaml::Media::Animation;

void SlideContentTransition::SetContent(ContentPresenter^ presenter, UIElement^ content, NavigationType navigationType)
{
	auto transitionCollection = presenter->ContentTransitions;
	TransitionCollectionHelper::ApplySingleTransition(transitionCollection, ref new PaneThemeTransition);
	presenter->ContentTransitions = transitionCollection;
	if (auto element = dynamic_cast<UIElement^>(presenter->Content)) element->Opacity = 0;
	content->Opacity = 1;
	presenter->Content = content;
}