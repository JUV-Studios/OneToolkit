#include "pch.h"

using namespace Platform;
using namespace XamlFramework;
using namespace Windows::Foundation;

namespace OneToolkit::UI::Xaml::Controls
{
	public ref struct XamlControlResources sealed : ResourceDictionary
	{
		XamlControlResources()
		{
			auto stylesDictionary = ref new ResourceDictionary;
			stylesDictionary->Source = ref new Uri("ms-appx:///OneToolkit.UI.Xaml/Styles.xaml");
			ResourceDictionary^ mergedDictionaries[] = { ref new Microsoft::UI::Xaml::Controls::XamlControlsResources, stylesDictionary };
			MergedDictionaries->ReplaceAll(ArrayReference<ResourceDictionary^>(mergedDictionaries, 2));
		}
	};
}