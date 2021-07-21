#pragma once
#include "Media.Animation.DefaultContentTransition.g.h"

namespace winrt::OneToolkit::UI::WinUI::Media::Animation
{
	namespace implementation
	{
		struct DefaultContentTransition : DefaultContentTransitionT<DefaultContentTransition>
		{
			void SetContent(Microsoft::UI::Xaml::Controls::ContentPresenter const& presenter, Microsoft::UI::Xaml::UIElement const& content, NavigationType navigationType);
		};
	}

	namespace factory_implementation
	{
		struct DefaultContentTransition : DefaultContentTransitionT<DefaultContentTransition, implementation::DefaultContentTransition>
		{
		};
	}
}