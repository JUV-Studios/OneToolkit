#pragma once
#include "NavigationType.h"

namespace OneToolkit::UI::Xaml::Media::Animation
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface struct IContentTransition
	{
		void SetContent(WUXC::ContentPresenter^ presenter, Windows::UI::Xaml::UIElement^ content, NavigationType navigationType);
	};
}