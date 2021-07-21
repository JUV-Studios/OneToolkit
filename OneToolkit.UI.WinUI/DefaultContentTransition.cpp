#include "pch.h"
#include "DefaultContentTransition.h"
#include "Media.Animation.DefaultContentTransition.g.cpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::OneToolkit::UI::WinUI::Media::Animation::implementation
{
	void DefaultContentTransition::SetContent(ContentPresenter const& presenter, UIElement const& content, NavigationType)
	{
		presenter.Content(content);
	}
}