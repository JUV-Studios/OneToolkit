#pragma once
#include "IContentTransition.h"

namespace Component::Media::Animation
{
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref struct DefaultContentTransition sealed : IContentTransition
	{
		virtual void SetContent(Framework::Controls::ContentPresenter^ presenter, Framework::UIElement^ content, NavigationType navigationType);
	};
}