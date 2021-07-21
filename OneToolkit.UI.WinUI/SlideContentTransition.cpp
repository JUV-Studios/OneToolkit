#include "pch.h"
#include "SlideContentTransition.h"
#include "Media.Animation.SlideContentTransition.g.cpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media::Animation;

namespace winrt::OneToolkit::UI::WinUI::Media::Animation::implementation
{
	void SlideContentTransition::SetContent(ContentPresenter const& presenter, UIElement const& content, NavigationType)
	{
		if (!presenter.ContentTransitions())
		{
			TransitionCollection transitionCollection;
			transitionCollection.Append(PaneThemeTransition());
			presenter.ContentTransitions(transitionCollection);
		}
		else if (presenter.ContentTransitions().Size() == 1)
		{
			if (presenter.ContentTransitions().GetAt(0).try_as<PaneThemeTransition>()) presenter.ContentTransitions().SetAt(0, PaneThemeTransition());
		}
		else
		{
			if (presenter.ContentTransitions().Size() > 1) presenter.ContentTransitions().Clear();
			presenter.ContentTransitions().Append(PaneThemeTransition());
		}

		double originalOpacity = 1;
		auto element = presenter.Content().try_as<UIElement>();
		if (element)
		{
			originalOpacity = element.Opacity();
			element.Opacity(0);
		}

		presenter.Content(content);
		if (element) element.Opacity(originalOpacity);
	}
}