#pragma once
#include "IContentTransition.h"

namespace Component::Media::Animation
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SlideContentTransition sealed : Framework::DependencyObject, IContentTransition
	{
	public:
		DeclareDependencyProperty(Framework::Controls::Primitives::EdgeTransitionLocation, BackEdge);

		DeclareDependencyProperty(Framework::Controls::Primitives::EdgeTransitionLocation, ForwardEdge);

		virtual void SetContent(Framework::Controls::ContentPresenter^ presenter, Framework::UIElement^ content, NavigationType navigationType);
	private:
		static void DependencyPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyPropertyChangedEventArgs ^ e);
	};
}