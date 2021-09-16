#include "pch.h"
#include "SlideContentTransition.h"

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace OneToolkit::UI::Xaml::Media::Animation;

EdgeTransitionLocation SlideContentTransition::BackEdge::get()
{
	return m_BackEdge;
}

void SlideContentTransition::BackEdge::set(EdgeTransitionLocation value)
{
	if (m_BackEdge != value)
	{
		m_BackEdge = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs("BackEdge"));
	}
}

EdgeTransitionLocation SlideContentTransition::ForwardEdge::get()
{
	return m_ForwardEdge;
}

void SlideContentTransition::ForwardEdge::set(EdgeTransitionLocation value)
{
	if (m_ForwardEdge != value)
	{
		m_ForwardEdge = value;
		PropertyChanged(this, ref new PropertyChangedEventArgs("ForwardEdge"));
	}
}

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

	// Set the right edge to animate from.
	dynamic_cast<PaneThemeTransition^>(presenter->ContentTransitions->GetAt(0))->Edge = navigationType == NavigationType::Back ? m_BackEdge : m_ForwardEdge;

	double originalOpacity = 1;
	auto element = dynamic_cast<UIElement^>(presenter->Content);
	auto eventArgs = ref new ContentTransitionCompletedEventArgs(presenter, element, content, navigationType);
	if (element)
	{
		originalOpacity = element->Opacity;
		element->Opacity = 0;
	}

	presenter->Content = content;
	if (element) element->Opacity = originalOpacity;
	ContentTransitionCompleted(this, eventArgs);
}