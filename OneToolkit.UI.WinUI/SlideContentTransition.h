#pragma once
#include "Media.Animation.SlideContentTransition.g.h"

namespace winrt::OneToolkit::UI::WinUI::Media::Animation
{
	namespace implementation
	{
		struct SlideContentTransition : SlideContentTransitionT<SlideContentTransition>
		{
			void SetContent(Microsoft::UI::Xaml::Controls::ContentPresenter const& presenter, Microsoft::UI::Xaml::UIElement const& content, NavigationType navigationType);
		};
	}

	namespace factory_implementation
	{
		struct SlideContentTransition :SlideContentTransitionT<SlideContentTransition, implementation::SlideContentTransition>
		{
		};
	}
}