#pragma once

namespace Component::Media::Animation
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface struct IContentTransition
	{
		void SetContent(Framework::Controls::ContentPresenter^ presenter, Framework::UIElement^ content, NavigationType navigationType);
	};
}