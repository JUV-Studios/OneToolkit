#pragma once

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface struct IContentEquatable
	{
		bool Equals(Windows::UI::Xaml::UIElement^ content);
	};
}