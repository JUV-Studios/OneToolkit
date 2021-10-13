#include "pch.h"
#include "SlideContentTransition.h"

using namespace Platform;
using namespace Framework;
using namespace Framework::Data;
using namespace	Framework::Controls;
using namespace Framework::Controls::Primitives;
using namespace Framework::Media::Animation;
using namespace Component::Media::Animation;

DefineDependencyProperty(EdgeTransitionLocation, SlideContentTransition, BackEdge, EdgeTransitionLocation::Left);

DefineDependencyProperty(EdgeTransitionLocation, SlideContentTransition, ForwardEdge, EdgeTransitionLocation::Right);

void SlideContentTransition::SetContent(ContentPresenter^ presenter, UIElement^ content, NavigationType navigationType)
{
	if (!presenter->ContentTransitions)
	{
		auto transitionCollection = ref new TransitionCollection;
		transitionCollection->Append(ref new PaneThemeTransition);
		presenter->ContentTransitions = transitionCollection;
	}
	else if (presenter->ContentTransitions->Size == 1)
	{
		if (!dynamic_cast<PaneThemeTransition^>(presenter->ContentTransitions->GetAt(0))) presenter->ContentTransitions->SetAt(0, ref new PaneThemeTransition);
	}
	else
	{
		if (presenter->ContentTransitions->Size > 1) presenter->ContentTransitions->Clear();
		presenter->ContentTransitions->Append(ref new PaneThemeTransition);
	}

	// Set the correct edge to animate from.
	dynamic_cast<PaneThemeTransition^>(presenter->ContentTransitions->GetAt(0))->Edge = navigationType == NavigationType::Back ? BackEdge : ForwardEdge;

	double originalOpacity = 1;
	auto element = dynamic_cast<UIElement^>(presenter->Content);
	if (element)
	{
		originalOpacity = element->Opacity;
		element->Opacity = 0;
	}

	presenter->Content = content;
	if (element) element->Opacity = originalOpacity;
}

void SlideContentTransition::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
}