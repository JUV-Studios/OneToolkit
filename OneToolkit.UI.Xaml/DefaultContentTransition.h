#pragma once
#include "IContentTransition.h"

namespace OneToolkit::UI::Xaml::Media::Animation
{
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref struct DefaultContentTransition sealed : IContentTransition
	{
		virtual event ContentTransitionCompletedEventHandler^ ContentTransitionCompleted;
		virtual void SetContent(WUXC::ContentPresenter^ presenter, Windows::UI::Xaml::UIElement^ content, NavigationType navigationType);
	};
}