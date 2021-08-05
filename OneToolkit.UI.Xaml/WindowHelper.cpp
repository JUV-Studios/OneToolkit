#include "pch.h"

using namespace Windows::UI::Xaml;

namespace OneToolkit::UI::Xaml
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class WindowHelper sealed
	{
	public:
		static WindowId GetWindowId(Window^ window)
		{
			return ViewService::GetCoreWindowId(window->CoreWindow);
		}
	private:
		WindowHelper();
	};
}