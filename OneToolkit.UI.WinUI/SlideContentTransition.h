#pragma once
#include "Media.Animation.SlideContentTransition.g.h"

namespace winrt::OneToolkit::UI::WinUI::Media::Animation
{
	namespace implementation
	{
		struct SlideContentTransition : SlideContentTransitionT<SlideContentTransition>, Mvvm::ObservableBase<SlideContentTransition, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs, Microsoft::UI::Xaml::Data::PropertyChangedEventHandler>
		{
			DeclareObservableProperty(Microsoft::UI::Xaml::Controls::Primitives::EdgeTransitionLocation, BackEdge, Microsoft::UI::Xaml::Controls::Primitives::EdgeTransitionLocation::Left);
			DeclareObservableProperty(Microsoft::UI::Xaml::Controls::Primitives::EdgeTransitionLocation, ForwardEdge, Microsoft::UI::Xaml::Controls::Primitives::EdgeTransitionLocation::Right);
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