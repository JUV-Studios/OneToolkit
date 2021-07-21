#include "pch.h"
#include "SlideContentTransition.h"

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace OneToolkit::UI::Xaml::Media::Animation;

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