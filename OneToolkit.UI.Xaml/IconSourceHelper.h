#pragma once

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class IconSourceHelper sealed
	{
	public:
		static WUXC::IconSource^ ToPlatform(MUXC::IconSource^ iconSource);
		static MUXC::IconSource^ ToLifted(WUXC::IconSource^ iconSource);
		static WUXC::IconSourceElement^ CreateElement(WUXC::IconSource^ iconSource);
	private:
		IconSourceHelper();
	};
}