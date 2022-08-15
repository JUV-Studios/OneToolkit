#include "pch.h"
#include "ContentDialogHelper.g.h"
#include <winrt/Windows.UI.Xaml.Media.h>

namespace winrt::OneToolkit::UI::Xaml::Controls
{
	namespace implementation
	{
		struct ContentDialogHelper : ContentDialogHelperT<ContentDialogHelper>
		{
			ContentDialogHelper() = delete;

			static Windows::UI::Xaml::Controls::ContentDialog GetCurrentlyShownDialog(Windows::UI::Xaml::Window const& window)
			{
				auto const openPopups = Windows::UI::Xaml::Media::VisualTreeHelper::GetOpenPopups(window);
				for (auto const& openPopup : openPopups)
				{
					if (auto const openContentDialog = openPopup.try_as<Windows::UI::Xaml::Controls::ContentDialog>())
					{
						return openContentDialog;
					}
				}

				return nullptr;
			}
		};
	}

	namespace factory_implementation
	{
		struct ContentDialogHelper : ContentDialogHelperT<ContentDialogHelper, implementation::ContentDialogHelper>
		{

		};
	}
}

#include "ContentDialogHelper.g.cpp"