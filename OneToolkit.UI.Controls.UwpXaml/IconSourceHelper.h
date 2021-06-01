#pragma once

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class IconSourceHelper sealed
	{
	public:
		static Windows::UI::Xaml::Controls::IconSource^ ToPlatform(Microsoft::UI::Xaml::Controls::IconSource^ iconSource);
		static Microsoft::UI::Xaml::Controls::IconSource^ ToLifted(Windows::UI::Xaml::Controls::IconSource^ iconSource);
		static Windows::UI::Xaml::Controls::IconSourceElement^ CreateElement(Windows::UI::Xaml::Controls::IconSource^ iconSource);
	private:
		IconSourceHelper();
	};
}