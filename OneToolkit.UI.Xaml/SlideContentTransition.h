#pragma once
#include "IContentTransition.h"

namespace OneToolkit::UI::Xaml::Media::Animation
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct SlideContentTransition sealed : IContentTransition, Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		property WUXC::Primitives::EdgeTransitionLocation BackEdge
		{
			WUXC::Primitives::EdgeTransitionLocation get();
			void set(WUXC::Primitives::EdgeTransitionLocation value);
		}

		property WUXC::Primitives::EdgeTransitionLocation ForwardEdge
		{
			WUXC::Primitives::EdgeTransitionLocation get();
			void set(WUXC::Primitives::EdgeTransitionLocation value);
		}

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		virtual void SetContent(WUXC::ContentPresenter^ presenter, Windows::UI::Xaml::UIElement^ content, NavigationType navigationType);
	private:
		WUXC::Primitives::EdgeTransitionLocation m_BackEdge = WUXC::Primitives::EdgeTransitionLocation::Left;
		WUXC::Primitives::EdgeTransitionLocation m_ForwardEdge = WUXC::Primitives::EdgeTransitionLocation::Right;
	};
}